#include "clientWorldInstance.h"
#include "clientActor.h"
#include "renderer.h"
#include "sprite.h"
#include "vector2d.h"

const double ClientWorldInstance::PIXELS_PER_WORLD_UNIT = 32.0;

ClientWorldInstance::ClientWorldInstance()
: _clientPlayerActor(0)
{
    // Input map
    _keyMap[SDLK_LEFT]  = KEY_LEFT;
    _keyMap[SDLK_RIGHT] = KEY_RIGHT;
    _keyMap[SDLK_UP]    = KEY_UP;
    _keyMap[SDLK_DOWN]  = KEY_DOWN;

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

void ClientWorldInstance::Render(double xCentre, double yCentre) const
{
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
    }
}

void ClientWorldInstance::Update(double elapsed)
{
    WorldInstance::Update(elapsed);

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
    if ( _clientPlayerActor )
        _clientPlayerActor->Move(v.x, v.y);
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

bool ClientWorldInstance::IsKeyDown(Key key) const
{
    assert(key >= 0 && key < KEY_NONE);
    return _keyDowns[key];
}