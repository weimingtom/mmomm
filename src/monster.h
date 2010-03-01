#ifndef MONSTER_H_
#define MONSTER_H_

#include "actor.h"
#include "clientSprites.h"

class Monster : public Actor {
public:

    Monster(const Rect& rect, ClientSprites::SpriteType animation);
    virtual ~Monster();

	virtual void update(double elapsed);
    virtual void onCollision(Physical& other);

private:

	// The sprite that the monster uses
	ClientSprites::SpriteType _animation;
	
	// Temporary data
	double _lastChange;
};

#endif