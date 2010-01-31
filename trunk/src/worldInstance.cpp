#include "worldInstance.h"
#include "worldMap.h"
#include "collision.h"
#include "actor.h"

WorldInstance* WorldInstance::_current = 0;

WorldInstance::WorldInstance()
{
    _collision = new TestCollision();
    _worldMap  = new WorldMap();
	_nextID = 0;
}

WorldInstance::~WorldInstance()
{
	while (!_actors.empty()) {
		delete _actors.begin()->second;
	}
	
    delete _collision;
    delete _worldMap;
}

const CollisionWorld& WorldInstance::GetCollision() const
{
    return *_collision;
}

CollisionWorld& WorldInstance::GetCollision()
{
    return *_collision;
}

const WorldMap& WorldInstance::GetWorldMap() const
{
    return *_worldMap;
}

WorldMap& WorldInstance::GetWorldMap()
{
    return *_worldMap;
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
