#include "clientWorldInstance.h"
#include "clientActor.h"
#include "renderer.h"

const double ClientWorldInstance::PIXELS_PER_WORLD_UNIT = 32.0;

ClientWorldInstance::ClientWorldInstance()
{
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
        AnimationManager::weak_ptr anim = a->GetAnimation();

        SDL_Rect clip;
        clip.x = anim.lock().get()->getCurrentFrameX();
        clip.y = anim.lock().get()->getCurrentFrameY();
        clip.w = anim.lock().get()->getFrameWidth();
        clip.h = anim.lock().get()->getFrameHeight();

        const Rect& r = a->GetCollisionRect();
        double tx = (PIXELS_PER_WORLD_UNIT * (r.left + r.right ) / 2.0 - xCentre) + (renderer.getScreenWidth()  - clip.w) / 2.0;
        double ty = (PIXELS_PER_WORLD_UNIT * (r.top  + r.bottom) / 2.0 - yCentre) + (renderer.getScreenHeight() - clip.h) / 2.0;
        if ( tx + clip.w < 0 || tx >= renderer.getScreenWidth() || ty + clip.h < 0 || ty >= renderer.getScreenHeight() )
            continue;
        renderer.drawClippedImage(anim.lock().get()->getImage().get(), float(tx), float(ty), clip);
    }
}