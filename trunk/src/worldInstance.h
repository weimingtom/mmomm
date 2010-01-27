#ifndef WORLDINSTANCE_H_
#define WORLDINSTANCE_H_

#include "collision.h"

class WorldMap;
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

};

#endif