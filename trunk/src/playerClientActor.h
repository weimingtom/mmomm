#ifndef PLAYER_CLIENT_ACTOR_H_
#define PLAYER_CLIENT_ACTOR_H_

#include "clientActor.h"

class PlayerClientActor : public ClientActor {
public:
	
    PlayerClientActor(ActorID actorID, const Rect& rect, const Vector2D& velocity, ClientSprites::SpriteType sprite);
    
    virtual void onCollision(Physical& other) { }
    Sprite* getSprite() const;
	
    virtual void update(double elapsed);
	
private:
};

#endif