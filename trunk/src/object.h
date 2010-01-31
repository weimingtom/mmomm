#ifndef OBJECT_H_
#define OBJECT_H_

#include "collision.h"
class WorldInstance;

// Object superclass common to both client and server.
class Object : public Physical {
public:

    Object(long objectId, const Rect& rect);
    Object(long objectId, double width, double height);

    virtual ~Object();

    long GetId() const;

private:

    long _objectId;

};

#endif