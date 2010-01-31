#include "playerActor.h"
#include "user.h"

PlayerActor::PlayerActor(unsigned long actorId, User& user, const Rect& rect)
: Actor(actorId, rect)
, _user(user)
{
}

PlayerActor::~PlayerActor()
{
}