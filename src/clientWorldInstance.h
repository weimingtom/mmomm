#ifndef CLIENTWORLDINSTANCE_H_
#define CLIENTWORLDINSTANCE_H_

#include "worldInstance.h"
#include <SDL/SDL.h>
#include <boost/unordered_map.hpp>

class ClientWorldMap;

class LocalCollision : public CollisionWorld {
public:

    virtual bool shouldBlock(const Physical* a, const Physical* b) const;

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
    void render();

	ClientWorldMap& getWorldMap() { return *_worldMap; }
	const ClientWorldMap& getWorldMap() const { return *_worldMap; }

    void keyDown(SDLKey key);
    void keyUp  (SDLKey key);
    void mouseDown(Uint8 key);
    void mouseUp  (Uint8 key);
    void mouseMotion(const Vector2D& offset);
    bool isKeyDown(Key key) const;
    const Vector2D& getWorldMouse() const;

    void   setClientPlayerActor(Actor* actor) { _clientPlayerActor = actor; }
    Actor* getClientPlayerActor() const       { return _clientPlayerActor; }

    virtual void update(double elapsed);

    std::vector< int >& getErroneousDestructionIds() { return _erroneousDestructionIds; }

private:

    Actor* _clientPlayerActor;
    int    _tileset;

    // Input
    typedef boost::unordered_map< SDLKey, Key >  KeyMap;
    KeyMap _keyMap;
    std::vector< bool > _keyDowns;

    std::vector< int > _erroneousDestructionIds;

    double   _updateOffset;
	Vector2D _camera;
    Vector2D _mouse;
    Vector2D _absoluteMouse;

	ClientWorldMap *_worldMap;
};

#endif
