#ifndef ACTOR_H_
#define ACTOR_H_

#include "collision.h"
class WorldInstance;

// Actor superclass common to both client and server.
class Actor : public Physical {
public:

    Actor(unsigned long actorId, const Rect& rect);
    Actor(unsigned long actorId, double width, double height);

    virtual ~Actor();

    unsigned long GetId() const;

private:

    unsigned long _actorId;

};

#endif