#include "clientObject.h"

ClientObject::ClientObject(const Rect& rect, ClientAnimations::Animation animation)
: Object( rect )
, _tempId(ClientAnimations::Get(animation))
, _animation(AnimationManager::current().getAnimation(_tempId))
{
}

ClientObject::~ClientObject()
{
    AnimationManager::current().deleteAnimation(_tempId);
}