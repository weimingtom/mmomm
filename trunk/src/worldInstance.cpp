#include "worldInstance.h"
#include "worldMap.h"
#include "collision.h"

WorldInstance::WorldInstance()
{
    _collision = new TestCollision();
    _worldMap  = new WorldMap();
}

WorldInstance::~WorldInstance()
{
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