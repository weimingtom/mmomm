#include "serverWorldInstance.h"
#include "user.h"
#include <iostream>

ServerWorldInstance::ServerWorldInstance()
{
}

ServerWorldInstance::~ServerWorldInstance()
{
}

void ServerWorldInstance::AddUser(User& user, PlayerActor* actor)
{
    _userMap[&user] = actor;
    std::cout << "stored player actor for " << user.username() << std::endl;
}

void ServerWorldInstance::RemoveUser(User& user)
{
    _userMap.erase(&user);
    std::cout << "removed player actor for " << user.username() << std::endl;
}

PlayerActor* ServerWorldInstance::GetUserActor(User& user) const
{
    UserMap::const_iterator i = _userMap.find(&user);
    if ( i == _userMap.end() )
        return 0;
    return i->second;
}

ServerWorldInstance& ServerWorldInstance::current()
{
    assert(dynamic_cast< ServerWorldInstance* >(&WorldInstance::current()));
    return *(ServerWorldInstance*)&WorldInstance::current();
}