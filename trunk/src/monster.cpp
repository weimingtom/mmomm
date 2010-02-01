#include "monster.h"

Monster::Monster(const Rect& rect, ClientSprites::SpriteType animation)
: Actor(rect)
, _animation(animation)
{
}

Monster::~Monster()
{
}
