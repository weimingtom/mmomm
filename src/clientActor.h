#ifndef CLIENTACTOR_H_
#define CLIENTACTOR_H_

#include "actor.h"
#include "clientAnimations.h"
#include "animationManager.h"
#include "hermite.h"

class ClientActor : public Actor {
public:

    ClientActor(ActorID actorID, const Rect& rect, const Vector2D& velocity, ClientAnimations::Animation animation);
    virtual ~ClientActor();

    virtual void OnCollision(Physical& other) { }
    AnimationManager::weak_ptr GetAnimation() const;

    virtual void Update();
	virtual void interpolate(double packetTime, const Vector2D& packetPosition, const Vector2D& packetVelocity);

private:

    int                        _tempID;
    AnimationManager::weak_ptr _animation;
	Vector2D                   _velocity;
	HermiteInterpolation	   _hermite;
	bool                       _useHermite;
};

#endif