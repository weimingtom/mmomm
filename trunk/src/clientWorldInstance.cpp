#include "clientWorldInstance.h"
#include <guichan.hpp>
#include "chatWindow.h"
#include "clientActor.h"
#include "clientWorldMap.h"
#include "collisionPackets.h"
#include "frameTimer.h"
#include "mouseSelectionMenu.h"
#include "networkClient.h"
#include "rect.h"
#include "renderer.h"
#include "sprite.h"
#include "vector2D.h"
#include "gui.h"

const double ClientWorldInstance::PIXELS_PER_WORLD_UNIT = 32.0;

class identifyLabelListener : public gcn::MouseListener
{
    private:
    std::string _text;
    gcn::Window *_window;

    class closeButtonListener : public gcn::MouseListener
    {
        private:
        gcn::Window *_window;

        public:
        closeButtonListener(gcn::Window *window)
        {
            _window = window;
        }

        void mouseClicked(gcn::MouseEvent &mouseEvent)
        {
            Gui::current().removeWidget(_window);
            delete _window;
        }
    };

    public:
    identifyLabelListener(std::string text)
    {
        _text = text;
    }

    void mouseClicked(gcn::MouseEvent &mouseEvent)
    {
        gcn::Window *_window = new gcn::Window("Identify");
        gcn::Label *label = new gcn::Label(_text);
        gcn::Button *button = new gcn::Button("close");

        label->setForegroundColor(gcn::Color(255, 255, 255));
        label->setPosition(0, 0);

        button->setPosition(0, 30);
        button->addMouseListener(new closeButtonListener(_window));

        _window->add(label);
        _window->add(button);
        _window->setPosition(0, 0);
        _window->setVisible(true);
        _window->resizeToContent();

        Gui::current().addWidget(_window);
    }
};

bool LocalCollision::shouldBlock(const Physical* a, const Physical* b) const
{
    return a == ClientWorldInstance::current().getClientPlayerActor() ||
           b == ClientWorldInstance::current().getClientPlayerActor();
}

ClientWorldInstance::ClientWorldInstance()
: WorldInstance(new LocalCollision())
, _clientPlayerActor(0)
, _updateOffset(0)
, _camera(0, 0)
, _worldMap(new ClientWorldMap())
{
    _tileset = AnimationManager::current().createAnimation(ImageManager::current().getImage("tileset.png"),
                                                           32, 32, 1, 0, -1, false);

    // Input map
    _keyMap[SDLK_LEFT]  = KEY_LEFT;
    _keyMap[SDLK_RIGHT] = KEY_RIGHT;
    _keyMap[SDLK_UP]    = KEY_UP;
    _keyMap[SDLK_DOWN]  = KEY_DOWN;
    _keyMap[SDLK_a]     = KEY_LEFT;
    _keyMap[SDLK_d]     = KEY_RIGHT;
    _keyMap[SDLK_w]     = KEY_UP;
    _keyMap[SDLK_s]     = KEY_DOWN;

    for ( std::size_t i = 0; i < KEY_NONE; i++ ) {
        _keyDowns.push_back(false);
    }
}

ClientWorldInstance::~ClientWorldInstance()
{
	delete _worldMap;
}

ClientWorldInstance& ClientWorldInstance::current()
{
    assert(dynamic_cast< ClientWorldInstance* >(&WorldInstance::current()));
    return *(ClientWorldInstance*)&WorldInstance::current();
}

void ClientWorldInstance::render()
{
    if ( _clientPlayerActor )
        _camera = Vector2D(.5 * (_clientPlayerActor->getCollisionRect().left + _clientPlayerActor->getCollisionRect().right ),
                           .5 * (_clientPlayerActor->getCollisionRect().top  + _clientPlayerActor->getCollisionRect().bottom));
    else
        _camera = Vector2D(0, 0);
	Vector2D centre = _camera;
    const ActorMap& actors = getActorMap();
    Renderer& renderer = Renderer::current();

	// First/past-the-end tiles to render.
    Rect camera = Rect(_camera - renderer.getScreenDimensions() * (.5 / PIXELS_PER_WORLD_UNIT),
                       _camera + renderer.getScreenDimensions() * (.5 / PIXELS_PER_WORLD_UNIT));
    IVector2D min(std::floor(camera.left), std::floor(camera.top));
	IVector2D max(std::ceil(camera.right), std::ceil(camera.bottom));

    AnimationManager::shared_ptr tileset = AnimationManager::current().getAnimation(_tileset);
	IVector2D i;
	for (i.y = min.y; i.y < max.y; ++i.y) {
		for (i.x = min.x; i.x < max.x; ++i.x) {
            tileset->setCurrentFrame(_worldMap->getSprite(i));
            SDL_Rect clip = tileset->getCurrentFrameRect();

		    Vector2D renderPosition = PIXELS_PER_WORLD_UNIT * (Vector2D(i.x, i.y) - centre) + .5 * (renderer.getScreenDimensions());
            renderer.drawClippedImage(tileset->getImage().get(), renderPosition, clip);
		}
	}

    // Render actors
    for ( ActorMap::const_iterator i = actors.begin(); i != actors.end(); i++ ) {
        assert(dynamic_cast<ClientActor*>(i->second));
        ClientActor *a = static_cast<ClientActor*>(i->second);
        AnimationManager::shared_ptr anim = a->getSprite()->getCurrentAnimation();

        SDL_Rect clip = anim->getCurrentFrameRect();

        const Rect& r = a->getCollisionRect();
		Vector2D renderPosition =
			PIXELS_PER_WORLD_UNIT * (.5 * (r.min() + r.max()) - centre) +
			.5 * (renderer.getScreenDimensions() - Vector2D(clip.w, clip.h));
        if (renderPosition.x + clip.w < 0 || renderPosition.x >= renderer.getScreenDimensions().x ||
			renderPosition.y + clip.h < 0 || renderPosition.y >= renderer.getScreenDimensions().y)
            continue;
        renderer.drawClippedImage(anim->getImage().get(), renderPosition, clip);

        /*if ( _mouse.x < r.right && _mouse.x >= r.left && _mouse.y < r.bottom && _mouse.y >= r.top ) {
            // Need a Renderer::drawText
            std::stringstream ss;
            ss << "That is ye " << a->GetName();
            ChatWindow::current().setText( ss.str() );
        }*/
    }
}

void ClientWorldInstance::update(double elapsed)
{
	if ( _clientPlayerActor ) {

		Vector2D v;
		if ( isKeyDown(KEY_LEFT) )
			v.x -= 1.0;
		if ( isKeyDown(KEY_RIGHT) )
			v.x += 1.0;
		if ( isKeyDown(KEY_UP) )
			v.y -= 1.0;
		if ( isKeyDown(KEY_DOWN) )
			v.y += 1.0;
		v = v.normalized();
		v *= 5.0;
		_clientPlayerActor->setVelocity(v);
	}

    WorldInstance::update(elapsed);

	if (!_clientPlayerActor)
		return;

    _updateOffset += 10.0 / FrameTimer::current().framerate();
    if ( _updateOffset >= 1.0 ) {
        _updateOffset -= int(_updateOffset);
        MovementPacket::MovementList list;
        MovementUpdate update;
        update.id = _clientPlayerActor->id();
        update.displacement = Vector2D(0, 0);
        update.velocity = _clientPlayerActor->getVelocity();
        list.push_back(update);
        MovementPacket movement(_clientPlayerActor->getPosition(), list.begin(), list.end());
        NetworkClient::current().send(movement);
    }
}

void ClientWorldInstance::keyDown(SDLKey key)
{
    KeyMap::const_iterator i = _keyMap.find(key);
    if ( i != _keyMap.end() )
        _keyDowns[i->second] = true;
}

void ClientWorldInstance::keyUp(SDLKey key)
{
    KeyMap::const_iterator i = _keyMap.find(key);
    if ( i != _keyMap.end() )
        _keyDowns[i->second] = false;
}

void ClientWorldInstance::mouseDown(Uint8 key)
{
    if ( key == SDL_BUTTON_LEFT )
        _keyDowns[KEY_LMB] = true;
    if ( key == SDL_BUTTON_RIGHT )
        _keyDowns[KEY_RMB] = true;
}

void ClientWorldInstance::mouseUp(Uint8 key)
{
    if ( key == SDL_BUTTON_LEFT )
        _keyDowns[KEY_LMB] = false;
    if ( key == SDL_BUTTON_RIGHT )
    {

        _keyDowns[KEY_RMB] = false;

        const ActorMap& actors = getActorMap();

        for ( ActorMap::const_iterator i = actors.begin(); i != actors.end(); i++ ) {
            assert(dynamic_cast< ClientActor* >(i->second));
            ClientActor *a = static_cast<ClientActor *>(i->second);

            const Rect& r = a->getCollisionRect();

            if( _mouse.x < r.right && _mouse.x >= r.left && _mouse.y < r.bottom && _mouse.y >= r.top ) {
                std::cout << "pos: " << _absoluteMouse.x << " " << _absoluteMouse.y << std::endl;
                MouseSelectionMenu *menu = new MouseSelectionMenu(int(_absoluteMouse.x), int(_absoluteMouse.y));
                MouseSelectionMenu::setCurrent(menu);

                gcn::Label *label = new gcn::Label("identify");
                label->addMouseListener(new identifyLabelListener(a->getName()));
                menu->addLabel(label);
            }
        }
    }
}

void ClientWorldInstance::mouseMotion(const Vector2D& offset)
{
	_mouse = _camera + (offset - .5 * Renderer::current().getScreenDimensions()) / PIXELS_PER_WORLD_UNIT;
	_absoluteMouse = offset;
}

bool ClientWorldInstance::isKeyDown(Key key) const
{
    assert(key >= 0 && key < KEY_NONE);
    return _keyDowns[key];
}

const Vector2D& ClientWorldInstance::getWorldMouse() const
{
    return _mouse;
}
