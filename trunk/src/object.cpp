#include "object.h"
#include "worldInstance.h"

Object::Object(const Rect& rect)
: Physical(WorldInstance::current().GetCollision(), rect)
{
}

Object::Object(double width, double height)
: Physical(WorldInstance::current().GetCollision(), width, height)
{
}

Object::~Object()
{
}