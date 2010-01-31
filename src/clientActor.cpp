#include "clientActor.h"

ClientActor::ClientActor(ActorID actorID, const Rect& rect, ClientAnimations::Animation animation)
: Actor(actorID, rect)
, _tempID(ClientAnimations::Get(animation))
, _animation(AnimationManager::current().getAnimation(_tempID))
{
}

ClientActor::~ClientActor()
{
    AnimationManager::current().deleteAnimation(_tempID);
}

AnimationManager::weak_ptr ClientActor::GetAnimation() const
{
    return _animation;
}
