#ifndef ACTOR_H_
#define ACTOR_H_

#include "collision.h"
#include "worldInstance.h"
#include "clientSprites.h"

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

    virtual void Update(double elapsed) { }
	
	// Tells the given actor to interpolate movement with the specified data.
	// Most actors do not need to implement this.
	// packetTime: absolute time on the packet
	// packetPosition: position of the actor at that time
	// packetVelocity: velocity of the actor at that time
	virtual void interpolate(double packetTime, const Vector2D& packetPosition, const Vector2D& packetVelocity) { }

    void GetNearbyActors(WorldInstance::ActorList& output) const;

    const Vector2D& GetVelocity() const { return _velocity; }
    void SetVelocity(const Vector2D& velocity) { _velocity = velocity; }

    ClientSprites::SpriteType GetSpriteType() const { return _spriteType; }
    void SetSpriteType(ClientSprites::SpriteType spriteType) { _spriteType = spriteType; }

    void SetName(const std::string& name) { _name = name; }
    const std::string& GetName() { return _name; }

private:

    Vector2D                  _velocity;
    ClientSprites::SpriteType _spriteType;
	
	void assignNewID();
	void addToWorld();
	void removeFromWorld();
	
    ActorID     _actorID;
    std::string _name;

};

#endif