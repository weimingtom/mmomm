#include "clientActor.h"
#include "frameTimer.h"
#include "sprite.h"

ClientActor::ClientActor(ActorID actorID, const Rect& rect, const Vector2D& velocity, ClientSprites::SpriteType sprite)
: Actor(actorID, rect)
, _sprite(ClientSprites::Get(sprite))
, _hermite()
, _useHermite(false)
{
	SetVelocity(velocity);
}

ClientActor::~ClientActor()
{
    delete _sprite;
}

Sprite* ClientActor::GetSprite() const
{
    return _sprite;
}

void ClientActor::Update(double elapsed)
{
	if (_useHermite) {
		SetPosition(_hermite.interpolatePosition(FrameTimer::current().frameTime()));
		if (FrameTimer::current().frameTime() >= _hermite.finalTime()) {
			_useHermite = false;
			SetVelocity(_hermite.interpolateVelocity(FrameTimer::current().frameTime()));
		}
	}
	else {
		SetPosition(GetPosition() + GetVelocity() * elapsed);
	}
}

void ClientActor::interpolate(double packetTime, const Vector2D& packetPosition, const Vector2D& packetVelocity)
{
	double currentTime = FrameTimer::current().frameTime();

	// Instantly move for small differences (less than a pixel)
	/*
	{
		Vector2D terp = packetPosition + (currentTime - packetTime) * packetVelocity;
		const double SMALL_DIFFERENCE = .5;
		if (terp.lengthSquared() < SMALL_DIFFERENCE * SMALL_DIFFERENCE) {
			_useHermite = false;
			SetPosition(terp);
			SetVelocity(packetVelocity);
			return;
		}
	}
	*/
	
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
		currentVelocity = _hermite.interpolateVelocity(currentTime);
	}
	else {
		currentPosition = GetPosition();
		currentVelocity = GetVelocity();
	}
	double elapsed = futureTime - packetTime;
	Vector2D futurePosition = packetPosition + elapsed * packetVelocity;
	_hermite = HermiteInterpolation(currentTime, futureTime,
		currentPosition, futurePosition,
		currentVelocity, packetVelocity);
	_useHermite = true;
}

void ClientActor::Move(double xOffset, double yOffset)
{
    Actor::Move(xOffset, yOffset);
    if ( xOffset == 0 && yOffset == 0 ) {
        _sprite->SetDefaultAnimation("stand");
    }
    else if ( abs(xOffset) >= abs(yOffset) ) {
        if ( xOffset >= 0 )
            _sprite->SetDefaultAnimation("walk-right");
        else
            _sprite->SetDefaultAnimation("walk-left");
    }
    else {
        if ( yOffset >= 0 )
            _sprite->SetDefaultAnimation("walk-down");
        else
            _sprite->SetDefaultAnimation("walk-up");
    }
}