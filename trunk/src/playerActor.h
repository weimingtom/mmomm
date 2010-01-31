#ifndef PLAYERACTOR_H_
#define PLAYERACTOR_H_

#include "actor.h"
class User;

class PlayerActor : public Actor {
public:

    PlayerActor(User& user, const Rect& rect);
    virtual ~PlayerActor();

    virtual void OnCollision(Physical& other) { }

private:

    User& _user;

};

#endif