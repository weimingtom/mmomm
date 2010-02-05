#include "actor.h"
#include "worldInstance.h"

Actor::Actor(const Rect& rect)
: Physical(WorldInstance::current().getCollision(), rect)
, _spriteType(ClientSprites::FIGHTER)
{
	assignNewID();
	addToWorld();
}

Actor::Actor(const Vector2D& dimensions)
: Physical(WorldInstance::current().getCollision(), dimensions)
{
	assignNewID();
	addToWorld();
}

Actor::Actor(unsigned long actorID, const Rect& rect)
: Physical(WorldInstance::current().getCollision(), rect)
, _actorID(actorID)
{
	addToWorld();
}

Actor::Actor(unsigned long actorID, const Vector2D& dimensions)
: Physical(WorldInstance::current().getCollision(), dimensions)
, _actorID(actorID)
{
	addToWorld();
}

Actor::~Actor()
{
	removeFromWorld();
}

void Actor::getNearbyActors(WorldInstance::ActorList& output) const
{
    const Rect& r = getCollisionRect();
    WorldInstance::current().getNearbyActors(output, (r.min() + r.max()) * .5);
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
