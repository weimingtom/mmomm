#include "clientWorldInstance.h"
#include "clientActor.h"
#include "renderer.h"
#include "sprite.h"
#include "vector2D.h"
#include "frameTimer.h"
#include "collisionPackets.h"
#include "networkClient.h"
#include "chatWindow.h"

const double ClientWorldInstance::PIXELS_PER_WORLD_UNIT = 32.0;

bool LocalCollision::ShouldBlock(const Physical* a, const Physical* b) const
{
    return a == ClientWorldInstance::current().GetClientPlayerActor() ||
           b == ClientWorldInstance::current().GetClientPlayerActor();
}

ClientWorldInstance::ClientWorldInstance()
: WorldInstance(new LocalCollision())
, _clientPlayerActor(0)
, _updateOffset(0)
, _xCam(0)
, _yCam(0)
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

void ClientWorldInstance::Render() const
{
    double xCentre = _xCam;
    double yCentre = _yCam;
    const ActorMap& actors = GetActorMap();
    Renderer& renderer = Renderer::current();

    for ( ActorMap::const_iterator i = actors.begin(); i != actors.end(); i++ ) {
        assert(dynamic_cast< ClientActor* >(i->second));
        ClientActor* a = (ClientActor*)i->second;
        AnimationManager::weak_ptr anim = a->GetSprite()->GetCurrentAnimation();

        SDL_Rect clip;
        clip.x = anim.lock().get()->getCurrentFrameX();
        clip.y = anim.lock().get()->getCurrentFrameY();
        clip.w = anim.lock().get()->getFrameWidth();
        clip.h = anim.lock().get()->getFrameHeight();

        const Rect& r = a->GetCollisionRect();
        double tx = PIXELS_PER_WORLD_UNIT * ((r.left + r.right ) / 2.0 - xCentre) + (renderer.getScreenWidth()  - clip.w) / 2.0;
        double ty = PIXELS_PER_WORLD_UNIT * ((r.top  + r.bottom) / 2.0 - yCentre) + (renderer.getScreenHeight() - clip.h) / 2.0;
        if ( tx + clip.w < 0 || tx >= renderer.getScreenWidth() || ty + clip.h < 0 || ty >= renderer.getScreenHeight() )
            continue;
        renderer.drawClippedImage(anim.lock().get()->getImage().get(), float(tx), float(ty), clip);

        if ( _mouse.x < r.right && _mouse.x >= r.left && _mouse.y < r.bottom && _mouse.y >= r.top ) {
            // Need a Renderer::drawText
            std::stringstream ss;
            ss << "That is ye " << a->GetName();
            ChatWindow::current().setText( ss.str() );
        }
    }
}

void ClientWorldInstance::Update(double elapsed)
{
    WorldInstance::Update(elapsed);

    if ( !_clientPlayerActor )
        return;

    Vector2D v;
    if ( IsKeyDown(KEY_LEFT) )
        v.x -= 1.0;
    if ( IsKeyDown(KEY_RIGHT) )
        v.x += 1.0;
    if ( IsKeyDown(KEY_UP) )
        v.y -= 1.0;
    if ( IsKeyDown(KEY_DOWN) )
        v.y += 1.0;
    v = v.normalized();
    v *= elapsed * 5.0;
    _clientPlayerActor->Move(v.x, v.y);

    _updateOffset += 10.0 / FrameTimer::current().framerate();
    if ( _updateOffset >= 1.0 ) {
        _updateOffset -= int(_updateOffset);
        MovementPacket::MovementList list;
        MovementUpdate update;
        update.id = _clientPlayerActor->id();
        update.position = _clientPlayerActor->GetPosition();
        update.velocity = _clientPlayerActor->GetVelocity();
        list.push_back(update);
        MovementPacket movement(list.begin(), list.end());
        NetworkClient::current().send(movement);
    }
}

void ClientWorldInstance::KeyDown(SDLKey key)
{
    KeyMap::const_iterator i = _keyMap.find(key);
    if ( i != _keyMap.end() )
        _keyDowns[i->second] = true;
}

void ClientWorldInstance::KeyUp(SDLKey key)
{
    KeyMap::const_iterator i = _keyMap.find(key);
    if ( i != _keyMap.end() )
        _keyDowns[i->second] = false;
}

void ClientWorldInstance::MouseDown(Uint8 key)
{
    if ( key == SDL_BUTTON_LEFT )
        _keyDowns[KEY_LMB] = true;
    if ( key == SDL_BUTTON_RIGHT )
        _keyDowns[KEY_RMB] = true;
}

void ClientWorldInstance::MouseUp(Uint8 key)
{
    if ( key == SDL_BUTTON_LEFT )
        _keyDowns[KEY_LMB] = false;
    if ( key == SDL_BUTTON_RIGHT )
        _keyDowns[KEY_RMB] = false;
}

void ClientWorldInstance::MouseMotion(Uint16 x, Uint16 y)
{
    _mouse.x = _xCam + ( x - Renderer::current().getScreenWidth()  / 2 ) / PIXELS_PER_WORLD_UNIT;
    _mouse.y = _yCam + ( y - Renderer::current().getScreenHeight() / 2 ) / PIXELS_PER_WORLD_UNIT;
}

bool ClientWorldInstance::IsKeyDown(Key key) const
{
    assert(key >= 0 && key < KEY_NONE);
    return _keyDowns[key];
}

const Vector2D& ClientWorldInstance::GetWorldMouse() const
{
    return _mouse;
}
