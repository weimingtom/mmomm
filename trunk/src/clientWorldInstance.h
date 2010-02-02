#ifndef CLIENTWORLDINSTANCE_H_
#define CLIENTWORLDINSTANCE_H_

#include "worldInstance.h"
#include <SDL/SDL.h>
#include <boost/unordered_map.hpp>

class LocalCollision : public CollisionWorld {
public:

    virtual bool ShouldBlock(const Physical* a, const Physical* b) const;

};

class ClientWorldInstance : public WorldInstance {
public:

    static const double PIXELS_PER_WORLD_UNIT;
    enum Key {
        KEY_LEFT,
        KEY_RIGHT,
        KEY_UP,
        KEY_DOWN,
        KEY_LMB,
        KEY_RMB,

        KEY_NONE
    };

    ClientWorldInstance();
    virtual ~ClientWorldInstance();

    static ClientWorldInstance& current();
    void Render() const;

    void KeyDown(SDLKey key);
    void KeyUp  (SDLKey key);
    void MouseDown(Uint8 key);
    void MouseUp  (Uint8 key);
    void MouseMotion(Uint16 x, Uint16 y);
    bool IsKeyDown(Key key) const;
    const Vector2D& GetWorldMouse() const;

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

    double   _updateOffset;
    double   _xCam;
    double   _yCam;
    Vector2D _mouse;

};

#endif