#ifndef WORLD_INSTANCE_H_
#define WORLD_INSTANCE_H_

#include "collision.h"

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
    ~WorldInstance();

    const CollisionWorld& GetCollision() const;
    /* */ CollisionWorld& GetCollision();

    const WorldMap& GetWorldMap() const;
    /* */ WorldMap& GetWorldMap();

    void AddActor( Actor* actor );

    virtual void Update() { }

    static WorldInstance& current() { assert(_current); return *_current; }
    static void setCurrent(WorldInstance* current) { _current = current; }

private:

    static WorldInstance *_current;

    CollisionWorld* _collision;
    WorldMap*       _worldMap;

    typedef std::vector< Actor* > ActorList;
    ActorList       _actors;

};

#endif
