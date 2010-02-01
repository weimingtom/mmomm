#ifndef WORLD_INSTANCE_H_
#define WORLD_INSTANCE_H_

#include "collision.h"
#include <boost/unordered_map.hpp>

class WorldMap;
class Actor;

class TestCollision : public CollisionWorld {
public:

    virtual bool ShouldBlock(Physical* a, Physical* b) const
    {
        return true;
    }

};

class WorldInstance {
public:

    WorldInstance();
    virtual ~WorldInstance();

    const CollisionWorld& GetCollision() const;
    /* */ CollisionWorld& GetCollision();

    const WorldMap& GetWorldMap() const;
    /* */ WorldMap& GetWorldMap();

    typedef boost::unordered_map< ActorID, Actor * > ActorMap;
    const ActorMap& GetActorMap() const;

    virtual void Update(double elapsed);

    static WorldInstance& current() { assert(_current); return *_current; }
    static void setCurrent(WorldInstance* current) { _current = current; }
	
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
