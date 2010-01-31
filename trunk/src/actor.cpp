#include "actor.h"
#include "worldInstance.h"

Actor::Actor(unsigned long actorId, const Rect& rect)
: Physical(WorldInstance::current().GetCollision(), rect)
, _actorId(actorId)
{
}

Actor::Actor(unsigned long actorId, double width, double height)
: Physical(WorldInstance::current().GetCollision(), width, height)
, _actorId(actorId)
{
}

Actor::~Actor()
{
}

unsigned long Actor::GetId() const
{
    return _actorId;
}