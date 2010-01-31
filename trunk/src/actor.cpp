#include "actor.h"
#include "worldInstance.h"

Actor::Actor(const Rect& rect)
: Physical(WorldInstance::current().GetCollision(), rect)
{
	assignNewID();
	addToWorld();
}

Actor::Actor(double width, double height)
: Physical(WorldInstance::current().GetCollision(), width, height)
{
	assignNewID();
	addToWorld();
}

Actor::Actor(unsigned long actorID, const Rect& rect)
: Physical(WorldInstance::current().GetCollision(), rect)
, _actorID(actorID)
{
	addToWorld();
}

Actor::Actor(unsigned long actorID, double width, double height)
: Physical(WorldInstance::current().GetCollision(), width, height)
, _actorID(actorID)
{
	addToWorld();
}

Actor::~Actor()
{
	removeFromWorld();
}

void Actor::assignNewID()
{
	_actorID = WorldInstance::current().generateID();
}

void Actor::addToWorld()
{
	WorldInstance::current().addActor(this);
}
void Actor::removeFromWorld()
{
	WorldInstance::current().removeActor(this);
}
