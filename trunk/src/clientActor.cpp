#include "clientActor.h"

ClientActor::ClientActor(long actorId, const Rect& rect, ClientAnimations::Animation animation)
: Actor(actorId, rect)
, _tempId(ClientAnimations::Get(animation))
, _animation(AnimationManager::current().getAnimation(_tempId))
{
}

ClientActor::~ClientActor()
{
    AnimationManager::current().deleteAnimation(_tempId);
}
