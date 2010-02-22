#ifndef SERVERWORLDINSTANCE_H_
#define SERVERWORLDINSTANCE_H_

#include "worldInstance.h"
#include <list>
class User;
class PlayerActor;
class Npc;
class NpcActor;
class ServerWorldMap;

class TestCollision : public CollisionWorld {
public:

    virtual bool shouldBlock(const Physical* a, const Physical* b) const
    {
        return true;
    }

};

class ServerWorldInstance : public WorldInstance {
public:

    ServerWorldInstance();
    virtual ~ServerWorldInstance();

    static ServerWorldInstance& current();

	ServerWorldMap& getWorldMap() { return *_worldMap; }
	const ServerWorldMap& getWorldMap() const { return *_worldMap; }

    void addUser(User& user, PlayerActor* actor);
    void removeUser(User& user);
    PlayerActor* getUserActor(User& user) const;

    typedef boost::unordered_map< User*, PlayerActor* > UserMap;
    typedef boost::unordered_map< Npc*, NpcActor* > NpcMap;
    const UserMap& getUserMap() const;

    virtual void update(double elapsed);

private:

    UserMap _userMap;
    typedef std::list< User* > UserList;
    UserList _userList;
    double _updatesOffset;

	ServerWorldMap *_worldMap;

};

#endif
