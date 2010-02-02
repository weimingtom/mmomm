#ifndef PLAYERACTOR_H_
#define PLAYERACTOR_H_

#include "actor.h"
class User;

class PlayerActor : public Actor {
public:

    PlayerActor(User& user, const Rect& rect);
    virtual ~PlayerActor();

    virtual void OnCollision(Physical& other) { }

	double getUpdateTime() const { return _updateTime; }
	void setUpdateTime(double updateTime) { _updateTime = updateTime; }

private:

    User& _user;
	double _updateTime;
};

#endif