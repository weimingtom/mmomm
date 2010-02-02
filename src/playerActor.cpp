#include "playerActor.h"
#include "user.h"
#include "frameTimer.h"

PlayerActor::PlayerActor(User& user, const Rect& rect)
: Actor(rect)
, _user(user)
, _updateTime(FrameTimer::current().frameTime())
{
}

PlayerActor::~PlayerActor()
{
}