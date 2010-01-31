#ifndef CLIENTOBJECT_H_
#define CLIENTOBJECT_H_

#include "object.h"
#include "clientAnimations.h"
#include "animationManager.h"

class ClientObject : public Object {
public:

    ClientObject(const Rect& rect, ClientAnimations::Animation animation);
    virtual ~ClientObject();

    virtual void OnCollision(Physical& other) { }

private:

    int                          _tempId;
    AnimationManager::shared_ptr _animation;

};

#endif