#ifndef CLIENTWORLDINSTANCE_H_
#define CLIENTWORLDINSTANCE_H_

#include "worldInstance.h"

class ClientWorldInstance : public WorldInstance {
public:

    static const double PIXELS_PER_WORLD_UNIT;

    ClientWorldInstance();
    virtual ~ClientWorldInstance();

    static ClientWorldInstance& current();

    // Renders the world. The camera is centred at the given world coordinates.
    void Render(double xCentre, double yCentre) const;

};

#endif