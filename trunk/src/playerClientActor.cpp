#include "playerClientActor.h"
#include "frameTimer.h"
#include "sprite.h"

PlayerClientActor::PlayerClientActor(
	ActorID actorID, const Rect& rect,
	const Vector2D& velocity, ClientSprites::SpriteType sprite)
: ClientActor(actorID, rect, sprite)
{
	setVelocity(velocity);
}

void PlayerClientActor::update(double elapsed)
{
	ClientActor::update(elapsed);
    ClientActor::move(getVelocity() * elapsed);
}
