#include "actor.h"
#include "worldInstance.h"

Actor::Actor(const Rect& rect)
: Physical(WorldInstance::current().GetCollision(), rect)
, _spriteType(ClientSprites::FIGHTER)
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

void Actor::GetNearbyActors(WorldInstance::ActorList& output) const
{
    const Rect& r = GetCollisionRect();
    WorldInstance::current().GetNearbyActors(output, Vector2D((r.left + r.right) / 2.0, (r.top + r.bottom) / 2.0));
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
