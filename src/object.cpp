#include "object.h"
#include "worldInstance.h"

Object::Object(long objectId, const Rect& rect)
: Physical(WorldInstance::current().GetCollision(), rect)
, _objectId(objectId)
{
}

Object::Object(long objectId, double width, double height)
: Physical(WorldInstance::current().GetCollision(), width, height)
, _objectId(objectId)
{
}

Object::~Object()
{
}

long Object::GetId() const
{
    return _objectId;
}