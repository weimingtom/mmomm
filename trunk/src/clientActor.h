#ifndef CLIENTACTOR_H_
#define CLIENTACTOR_H_

#include "actor.h"
#include "clientSprites.h"
#include "animationManager.h"
#include "hermite.h"
class Sprite;

class ClientActor : public Actor {
public:

    ClientActor(ActorID actorID, const Rect& rect, const Vector2D& velocity, ClientSprites::SpriteType sprite);
    virtual ~ClientActor();

    virtual void OnCollision(Physical& other) { }
    Sprite* GetSprite() const;

    virtual void Move(double xOffset, double yOffset);
    virtual void Update(double elapsed);
	virtual void interpolate(double packetTime, const Vector2D& packetPosition, const Vector2D& packetVelocity);

private:

    int                        _tempID;
    Sprite*                    _sprite;
	HermiteInterpolation	   _hermite;
	bool                       _useHermite;
};

#endif