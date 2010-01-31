#include "clientObject.h"

ClientObject::ClientObject(WorldInstance& world, const Rect& rect, ClientAnimations::Animation animation)
: Object( world, rect )
, _tempId(ClientAnimations::Get(animation))
, _animation(AnimationManager::current().getAnimation(_tempId))
{
}

ClientObject::~ClientObject()
{
    AnimationManager::current().deleteAnimation(_tempId);
}