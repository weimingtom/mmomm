#include "playerActor.h"
#include "user.h"

PlayerActor::PlayerActor(User& user, const Rect& rect)
: Actor(rect)
, _user(user)
{
}

PlayerActor::~PlayerActor()
{
}