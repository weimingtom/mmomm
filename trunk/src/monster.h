#ifndef MONSTER_H_
#define MONSTER_H_

#include "actor.h"
#include "clientAnimations.h"

class Monster : public Actor {
public:

    Monster(const Rect& rect, ClientAnimations::Animation animation);
    virtual ~Monster();

    virtual void OnCollision(Physical& other) { }

private:

	// The sprite that the monster uses
	ClientAnimations::Animation _animation;
};

#endif