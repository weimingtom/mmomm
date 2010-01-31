#ifndef ACTOR_H_
#define ACTOR_H_

#include "collision.h"
class WorldInstance;

// Actor superclass common to both client and server.
class Actor : public Physical {
public:

    Actor(long actorId, const Rect& rect);
    Actor(long actorId, double width, double height);

    virtual ~Actor();

    long GetId() const;

private:

    long _actorId;

};

#endif