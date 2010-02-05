#ifndef WORLD_INSTANCE_H_
#define WORLD_INSTANCE_H_

#include "collision.h"
#include <boost/unordered_map.hpp>

class WorldMap;
class Actor;

class WorldInstance {
public:

    WorldInstance(CollisionWorld* collision);
    virtual ~WorldInstance();

    const CollisionWorld& getCollision() const;
    /* */ CollisionWorld& getCollision();

    const WorldMap& getWorldMap() const;
    /* */ WorldMap& getWorldMap();

    typedef boost::unordered_map< ActorID, Actor * > ActorMap;
    const ActorMap& getActorMap() const;

    virtual void update(double elapsed);

    static WorldInstance& current() { assert(_current); return *_current; }
    static void setCurrent(WorldInstance* current) { _current = current; }

    typedef std::vector< Actor* > ActorList;
    void getNearbyActors(ActorList& output, const Vector2D& point);
	
private:
	
    static WorldInstance *_current;

    CollisionWorld* _collision;
    WorldMap*       _worldMap;
	
    ActorMap        _actors;

	// The actorID to use next
	mutable ActorID	_nextID;

	// Add/remove the specified actor from the map.
	// Called automatically.
	friend class Actor;
	void addActor(Actor* actor);
	void removeActor(Actor* actor);

	// Find an unused actor id
	ActorID generateID() const;
};

#endif
