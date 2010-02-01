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
}

void ServerWorldInstance::RemoveUser(User& user)
{
    _userMap.erase(&user);
}

PlayerActor* ServerWorldInstance::GetUserActor(User& user) const
{
    UserMap::const_iterator i = _userMap.find(&user);
    if ( i == _userMap.end() )
        return 0;
    return i->second;
}

const ServerWorldInstance::UserMap& ServerWorldInstance::GetUserMap() const
{
    return _userMap;
}

ServerWorldInstance& ServerWorldInstance::current()
{
    assert(dynamic_cast< ServerWorldInstance* >(&WorldInstance::current()));
    return *(ServerWorldInstance*)&WorldInstance::current();
}