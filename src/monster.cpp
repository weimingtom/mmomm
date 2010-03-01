#include "monster.h"
#include "frameTimer.h"
#include "random.h"

Monster::Monster(const Rect& rect, ClientSprites::SpriteType animation)
: Actor(rect)
, _animation(animation)
, _lastChange(FrameTimer::current().now())
{
}

Monster::~Monster()
{
}

void Monster::update(double elapsed)
{
	// Change direction every second or so.
	if (FrameTimer::current().now() - _lastChange > Random::real(1, 3)) {
		_lastChange = FrameTimer::current().now();
		setVelocity(Vector2D(Random::real(-1, 1), Random::real(-2, 2)));
	}
	move(getVelocity() * elapsed);
}

void Monster::onCollision(Physical& other)
{
	// On collision, stop.
	setVelocity(Vector2D(0, 0));
}