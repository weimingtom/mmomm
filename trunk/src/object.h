#ifndef OBJECT_H_
#define OBJECT_H_

#include "collision.h"
class WorldInstance;

// Object superclass common to both client and server.
class Object : public Physical {
public:

    Object(const Rect& rect);
    Object(double width, double height);

    virtual ~Object();

};

#endif