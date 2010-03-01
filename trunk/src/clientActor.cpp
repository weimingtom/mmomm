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

	AnimationManager::current().setAnimationActive(_sprite->getCurrentAnimation().get()->getId(), false);

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

	/*if(getVelocity().length() == 0)
        AnimationManager::current().setAnimationActive(_sprite->getCurrentAnimation().get()->getId(), false);
	else
	    AnimationManager::current().setAnimationActive(_sprite->getCurrentAnimation().get()->getId(), true);*/

    if (getVelocity().lengthSquared() < EPSILON * EPSILON) {
        _sprite->setDefaultAnimation("stand");
        if(_sprite->getCurrentAnimation().get()->isActive())
            AnimationManager::current().setAnimationActive(_sprite->getCurrentAnimation().get()->getId(), false);
    }
    else if (abs(getVelocity().x) + EPSILON > abs(getVelocity().y)) {
        if (getVelocity().x >= 0)
            _sprite->setDefaultAnimation("walk-right");
        else
            _sprite->setDefaultAnimation("walk-left");
        if(!_sprite->getCurrentAnimation().get()->isActive())
            AnimationManager::current().setAnimationActive(_sprite->getCurrentAnimation().get()->getId(), true);
    }
    else {
        if (getVelocity().y >= 0)
            _sprite->setDefaultAnimation("walk-down");
        else
            _sprite->setDefaultAnimation("walk-up");
        if(!_sprite->getCurrentAnimation().get()->isActive())
            AnimationManager::current().setAnimationActive(_sprite->getCurrentAnimation().get()->getId(), true);
    }
}

