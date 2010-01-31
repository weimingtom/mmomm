#ifndef PLAYERACTOR_H_
#define PLAYERACTOR_H_

#include "actor.h"
class User;

class PlayerActor : public Actor {
public:

    PlayerActor(unsigned long actorId, User& user, const Rect& rect);
    virtual ~PlayerActor();

private:

    User& _user;

};

#endif