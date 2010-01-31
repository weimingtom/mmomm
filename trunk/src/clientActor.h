#ifndef CLIENTACTOR_H_
#define CLIENTACTOR_H_

#include "actor.h"
#include "clientAnimations.h"
#include "animationManager.h"

class ClientActor : public Actor {
public:

    ClientActor(long actorId, const Rect& rect, ClientAnimations::Animation animation);
    virtual ~ClientActor();

    virtual void OnCollision(Physical& other) { }

private:

    int                        _tempId;
    AnimationManager::weak_ptr _animation;

};

#endif