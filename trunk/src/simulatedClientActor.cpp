#include "simulatedClientActor.h"
#include "frameTimer.h"

SimulatedClientActor::SimulatedClientActor(
	ActorID actorID, const Rect& rect,
	const Vector2D& velocity,
    ClientSprites::SpriteType sprite)
: ClientActor(actorID, rect, sprite)
, _hermite()
, _useHermite(false)
{
	setVelocity(velocity);
}

void SimulatedClientActor::update(double elapsed)
{
	// Update position/velocity
	if (_useHermite) {
        setPosition(_hermite.interpolatePosition(FrameTimer::current().frameTime()));
		setVelocity(_hermite.interpolateVelocity(FrameTimer::current().frameTime()));
		if (FrameTimer::current().frameTime() >= _hermite.finalTime()) {
			_useHermite = false;
		}
	}
	else {
        setPosition(getPosition() + getVelocity() * elapsed);
	}
	
	ClientActor::update(elapsed);
}

void SimulatedClientActor::interpolate(double packetTime, const Vector2D& packetPosition, const Vector2D& packetVelocity)
{
	double currentTime = FrameTimer::current().frameTime();

	// Instantly move for small differences (less than a pixel)
	/*
	{
		Vector2D terp = packetPosition + (currentTime - packetTime) * packetVelocity;
		const double SMALL_DIFFERENCE = .5;
		if (terp.lengthSquared() < SMALL_DIFFERENCE * SMALL_DIFFERENCE) {
			_useHermite = false;
			setPosition(terp);
			setVelocity(packetVelocity);
			return;
		}
	}
	*/
	
	const double INTERPOLATION_TIME = .25;
	double futureTime = currentTime + INTERPOLATION_TIME;
	if (packetTime > futureTime)
		futureTime = packetTime;
	Vector2D currentPosition;
	Vector2D currentVelocity;
	if (_useHermite) {
		// Doesn't matter if hermite has expired

		// We already got a better packet
		if (_hermite.finalTime() >= futureTime) {
			return;
		}
		currentPosition = _hermite.interpolatePosition(currentTime);
		currentVelocity = _hermite.interpolateVelocity(currentTime);
	}
	else {
		currentPosition = getPosition();
		currentVelocity = getVelocity();
	}
	double elapsed = futureTime - packetTime;
	Vector2D futurePosition = packetPosition + elapsed * packetVelocity;
	_hermite = HermiteInterpolation(currentTime, futureTime,
		currentPosition, futurePosition,
		currentVelocity, packetVelocity);
	_useHermite = true;
}
