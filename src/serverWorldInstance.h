#ifndef SERVERWORLDINSTANCE_H_
#define SERVERWORLDINSTANCE_H_

#include "worldInstance.h"
class User;
class PlayerActor;

class ServerWorldInstance : public WorldInstance {
public:

    ServerWorldInstance();
    virtual ~ServerWorldInstance();

    static ServerWorldInstance& current();

    void AddUser(User& user, PlayerActor* actor);
    void RemoveUser(User& user);
    PlayerActor* GetUserActor(User& user) const;

    typedef boost::unordered_map< User*, PlayerActor* > UserMap;
    const UserMap& GetUserMap() const;

private:

    UserMap _userMap;

};

#endif