#include "worldInstance.h"
#include "worldMap.h"
#include "collision.h"
#include "actor.h"

WorldInstance* WorldInstance::_current = 0;

WorldInstance::WorldInstance(CollisionWorld* collision)
: _collision(collision)
, _worldMap(new WorldMap())
, _nextID(0)
{
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

const WorldInstance::ActorMap& WorldInstance::GetActorMap() const
{
    return _actors;
}

void WorldInstance::Update(double elapsed)
{
    for ( ActorMap::iterator i = _actors.begin(); i != _actors.end(); i++ ) {
        i->second->Update(elapsed);
    }
}

void WorldInstance::GetNearbyActors(ActorList& output, const Vector2D& point)
{
    _collision->GetNearbyPhysicals(output, point.x, point.y);
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
