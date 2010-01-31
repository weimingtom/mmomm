#ifndef ACTOR_H_
#define ACTOR_H_

#include "collision.h"
class WorldInstance;

// Actor superclass common to both client and server.
class Actor : public Physical {
public:

	// Server-only autoassign ID
    Actor(const Rect& rect);
    Actor(double width, double height);
	// Client-only
    Actor(unsigned long actorID, const Rect& rect);
    Actor(unsigned long actorID, double width, double height);

    virtual ~Actor();

	ActorID id() const { return _actorID; }

    virtual void Update() { }

private:
	
	void assignNewID();
	void addToWorld();
	void removeFromWorld();
	
    ActorID _actorID;

};

#endif