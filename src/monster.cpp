#include "monster.h"

Monster::Monster(const Rect& rect, ClientAnimations::Animation animation)
: Actor(rect)
, _animation(animation)
{
}

Monster::~Monster()
{
}
