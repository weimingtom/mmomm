#ifndef WORLD_INSTANCE_H_
#define WORLD_INSTANCE_H_

#include "collision.h"

class WorldMap;
class Object;

class TestCollision : public CollisionWorld {
public:

    virtual bool ShouldBlock( Physical* a, Physical* b ) const
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

private:

    CollisionWorld* _collision;
    WorldMap*       _worldMap;

    typedef std::vector< Object* > ObjectList;
    ObjectList      _objects;

};

#endif
