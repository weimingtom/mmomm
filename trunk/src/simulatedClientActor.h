#ifndef SIMULATED_CLIENT_ACTOR_H_
#define SIMULATED_CLIENT_ACTOR_H_

#include "clientActor.h"
#include "hermite.h"
class Sprite;

class SimulatedClientActor : public ClientActor {
public:
	
    SimulatedClientActor(ActorID actorID, const Rect& rect, const Vector2D& velocity,
	                     ClientSprites::SpriteType sprite);
	
    virtual void onCollision(Physical& other) { }
	
    virtual void update(double elapsed);
	virtual void interpolate(double packetTime, const Vector2D& packetPosition, const Vector2D& packetVelocity);
	
private:
	
	HermiteInterpolation	   _hermite;
	bool                       _useHermite;
};

#endif
