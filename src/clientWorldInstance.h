#ifndef CLIENTWORLDINSTANCE_H_
#define CLIENTWORLDINSTANCE_H_

#include "worldInstance.h"
#include <SDL/SDL.h>
#include <boost/unordered_map.hpp>

class NoCollision : public CollisionWorld {
public:

    virtual bool ShouldBlock(Physical* a, Physical* b) const
    {
        return false;
    }

};

class ClientWorldInstance : public WorldInstance {
public:

    static const double PIXELS_PER_WORLD_UNIT;
    enum Key {
        KEY_LEFT,
        KEY_RIGHT,
        KEY_UP,
        KEY_DOWN,

        KEY_NONE
    };

    ClientWorldInstance();
    virtual ~ClientWorldInstance();

    static ClientWorldInstance& current();

    // Renders the world. The camera is centred at the given world coordinates.
    void Render(double xCentre, double yCentre) const;

    void KeyDown(SDLKey key);
    void KeyUp  (SDLKey key);
    bool IsKeyDown(Key key) const;

    void   SetClientPlayerActor(Actor* actor) { _clientPlayerActor = actor; }
    Actor* GetClientPlayerActor() const       { return _clientPlayerActor; }

    virtual void Update(double elapsed);

    std::vector< int >& GetErroneousDestructionIds() { return _erroneousDestructionIds; }

private:

    Actor* _clientPlayerActor;

    // Input
    typedef boost::unordered_map< SDLKey, Key >  KeyMap;
    KeyMap _keyMap;
    std::vector< bool > _keyDowns;

    std::vector< int > _erroneousDestructionIds;

    double _updateOffset;

};

#endif