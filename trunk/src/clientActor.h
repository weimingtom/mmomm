#ifndef CLIENT_ACTOR_H_
#define CLIENT_ACTOR_H_

#include "actor.h"
#include "clientSprites.h"
#include "animationManager.h"
class Sprite;

class ClientActor : public Actor {
public:

    ClientActor(ActorID actorID, const Rect& rect, ClientSprites::SpriteType sprite);
    virtual ~ClientActor();

	void setSprite(ClientSprites::SpriteType);
	Sprite* getSprite() const { return _sprite; }

	virtual void update(double elapsed);

private:

	void updateSprite();

    Sprite*                    _sprite;
};

#endif