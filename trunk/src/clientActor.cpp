#include "clientActor.h"
#include "frameTimer.h"
#include "sprite.h"

ClientActor::ClientActor(ActorID actorID, const Rect& rect,
                         ClientSprites::SpriteType sprite)
: Actor(actorID, rect)
, _sprite()
{
	setSprite(sprite);
}

ClientActor::~ClientActor()
{
    delete _sprite;
}

void ClientActor::setSprite(ClientSprites::SpriteType sprite)
{
	Sprite *old = _sprite;
	_sprite = ClientSprites::Get(sprite);	
	delete old;

	setSpriteType(sprite);

	updateSprite();
}

void ClientActor::update(double elapsed)
{
	Actor::update(elapsed);

	updateSprite();
}

void ClientActor::updateSprite()
{
	// Update animation
	double EPSILON = .01;
    if (getVelocity().lengthSquared() < EPSILON * EPSILON) {
        _sprite->setDefaultAnimation("stand");
    }
    else if (abs(getVelocity().x) + EPSILON > abs(getVelocity().y)) {
        if (getVelocity().x >= 0)
            _sprite->setDefaultAnimation("walk-right");
        else
            _sprite->setDefaultAnimation("walk-left");
    }
    else {
        if (getVelocity().y >= 0)
            _sprite->setDefaultAnimation("walk-down");
        else
            _sprite->setDefaultAnimation("walk-up");
    }
}

