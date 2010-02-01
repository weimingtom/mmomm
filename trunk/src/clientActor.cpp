#include "clientActor.h"
#include "frameTimer.h"
#include "sprite.h"

ClientActor::ClientActor(ActorID actorID, const Rect& rect, const Vector2D& velocity, ClientSprites::SpriteType sprite)
: Actor(actorID, rect)
, _sprite(ClientSprites::Get(sprite))
, _velocity(velocity)
, _hermite()
, _useHermite(false)
{
    // TEST!
    _sprite->PlayAnimation( "walk-right", 4 );
}

ClientActor::~ClientActor()
{
    delete _sprite;
}

Sprite* ClientActor::GetSprite() const
{
    return _sprite;
}

void ClientActor::Update()
{
	if (_useHermite) {
		SetPosition(_hermite.interpolatePosition(FrameTimer::current().frameTime()));
		if (FrameTimer::current().frameTime() >= _hermite.finalTime()) {
			_useHermite = false;
		}
	}
	else {
		SetPosition(GetPosition() + _velocity * FrameTimer::current().elapsed());
	}
}

void ClientActor::interpolate(double packetTime, const Vector2D& packetPosition, const Vector2D& packetVelocity)
{
	double currentTime = FrameTimer::current().frameTime();
	const double INTERPOLATION_TIME = .1;
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
		currentVelocity = _hermite.interpolatePosition(futureTime);
	}
	else {
		currentPosition = GetPosition();
		currentVelocity = _velocity;
	}
	double elapsed = futureTime - packetTime;
	Vector2D futurePosition = futurePosition + elapsed * packetVelocity;
	_hermite = HermiteInterpolation(currentTime, futureTime,
		currentPosition, futurePosition,
		currentVelocity, packetVelocity);
	_useHermite = true;
}

