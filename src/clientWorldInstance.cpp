#include "clientWorldInstance.h"
#include "clientActor.h"
#include "renderer.h"

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
        double tx = (r.left + r.right  - clip.w + renderer.getScreenWidth() ) / 2.0 - xCentre;
        double ty = (r.top  + r.bottom - clip.h + renderer.getScreenHeight()) / 2.0 - yCentre;
        if ( tx + clip.w < 0 || tx >= renderer.getScreenWidth() || ty + clip.h < 0 || ty >= renderer.getScreenHeight() )
            continue;
        renderer.drawClippedImage(anim.lock().get()->getImage().get(), float(tx), float(ty), clip);
    }
}