#include "clientWorldInstance.h"
#include "clientActor.h"
#include "renderer.h"
#include "sprite.h"
#include "vector2D.h"
#include "frameTimer.h"
#include "collisionPackets.h"
#include "networkClient.h"
#include "chatWindow.h"
#include "rect.h"

const double ClientWorldInstance::PIXELS_PER_WORLD_UNIT = 32.0;

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
{
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
}

ClientWorldInstance& ClientWorldInstance::current()
{
    assert(dynamic_cast< ClientWorldInstance* >(&WorldInstance::current()));
    return *(ClientWorldInstance*)&WorldInstance::current();
}

void ClientWorldInstance::render() const
{
	Vector2D centre = _camera;
    const ActorMap& actors = getActorMap();
    Renderer& renderer = Renderer::current();

    for ( ActorMap::const_iterator i = actors.begin(); i != actors.end(); i++ ) {
        assert(dynamic_cast< ClientActor* >(i->second));
        ClientActor *a = static_cast<ClientActor *>(i->second);
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

    const ActorMap& actors = getActorMap();

    for ( ActorMap::const_iterator i = actors.begin(); i != actors.end(); i++ ) {
        assert(dynamic_cast< ClientActor* >(i->second));
        ClientActor *a = static_cast<ClientActor *>(i->second);

        const Rect& r = a->getCollisionRect();

        if( _mouse.x < r.right && _mouse.x >= r.left && _mouse.y < r.bottom && _mouse.y >= r.top ) {

        }
    }
}

void ClientWorldInstance::mouseUp(Uint8 key)
{
    if ( key == SDL_BUTTON_LEFT )
        _keyDowns[KEY_LMB] = false;
    if ( key == SDL_BUTTON_RIGHT )
        _keyDowns[KEY_RMB] = false;
}

void ClientWorldInstance::mouseMotion(const Vector2D& offset)
{
	_mouse = _camera + (offset - .5 * Renderer::current().getScreenDimensions()) / PIXELS_PER_WORLD_UNIT;
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
