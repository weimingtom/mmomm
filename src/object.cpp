#include "object.h"
#include "worldInstance.h"

Object::Object(WorldInstance& worldInstance, const Rect& rect)
: Physical(worldInstance.GetCollision(), rect)
{
}

Object::Object(WorldInstance& worldInstance, double width, double height)
: Physical(worldInstance.GetCollision(), width, height)
{
}

Object::~Object()
{
}