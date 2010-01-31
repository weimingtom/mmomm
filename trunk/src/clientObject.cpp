#include "clientObject.h"

ClientObject::ClientObject(long objectId, const Rect& rect, ClientAnimations::Animation animation)
: Object(objectId, rect)
, _tempId(ClientAnimations::Get(animation))
, _animation(AnimationManager::current().getAnimation(_tempId))
{
}

ClientObject::~ClientObject()
{
    AnimationManager::current().deleteAnimation(_tempId);
}