#ifndef CLIENTWORLDINSTANCE_H_
#define CLIENTWORLDINSTANCE_H_

#include "worldInstance.h"

class ClientWorldInstance : public WorldInstance {
public:

    ClientWorldInstance();
    virtual ~ClientWorldInstance();

    static ClientWorldInstance& current();

    // Renders the world. The camera is centred at the given coordinates.
    void Render(double xCentre, double yCentre) const;

};

#endif