#include "worldInstance.h"
#include "collision.h"
#include "actor.h"

WorldInstance* WorldInstance::_current = 0;

WorldInstance::WorldInstance(CollisionWorld* collision)
: _collision(collision)
, _nextID(0)
{
}

WorldInstance::~WorldInstance()
{
	while (!_actors.empty()) {
		delete _actors.begin()->second;
	}
	
    delete _collision;
}

const CollisionWorld& WorldInstance::getCollision() const
{
    return *_collision;
}

CollisionWorld& WorldInstance::getCollision()
{
    return *_collision;
}

const WorldInstance::ActorMap& WorldInstance::getActorMap() const
{
    return _actors;
}

void WorldInstance::update(double elapsed)
{
    for ( ActorMap::iterator i = _actors.begin(); i != _actors.end(); i++ ) {
        i->second->update(elapsed);
    }
}

void WorldInstance::getNearbyActors(ActorList& output, const Vector2D& point)
{
    _collision->getNearbyPhysicals(output, point);
}

void WorldInstance::addActor(Actor* actor)
{
	assert(!_actors.count(actor->id()));
    _actors[actor->id()] = actor;
}

void WorldInstance::removeActor(Actor* actor)
{
	assert(_actors.count(actor->id()));
	_actors.erase(actor->id());
}

ActorID WorldInstance::generateID() const
{
	// NOTE: Could lead to infinite loop on 64 bit systems,
	// but that's highly improbable.
	while (_actors.count(_nextID)) {
		++_nextID;
	}
	return _nextID++;
}
