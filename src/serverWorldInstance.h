#ifndef SERVERWORLDINSTANCE_H_
#define SERVERWORLDINSTANCE_H_

#include "worldInstance.h"
#include <list>
class User;
class PlayerActor;

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

    void addUser(User& user, PlayerActor* actor);
    void removeUser(User& user);
    PlayerActor* getUserActor(User& user) const;

    typedef boost::unordered_map< User*, PlayerActor* > UserMap;
    const UserMap& getUserMap() const;

    virtual void update(double elapsed);

private:

    UserMap _userMap;
    typedef std::list< User* > UserList;
    UserList _userList;
    double _updatesOffset;

};

#endif