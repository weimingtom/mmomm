#include "serverWorldInstance.h"
#include "user.h"
#include "frameTimer.h"
#include "playerActor.h"
#include <iostream>

ServerWorldInstance::ServerWorldInstance()
: _updatesOffset(0)
{
}

ServerWorldInstance::~ServerWorldInstance()
{
}

void ServerWorldInstance::AddUser(User& user, PlayerActor* actor)
{
    _userMap[&user] = actor;
    _userList.push_front(&user);
}

void ServerWorldInstance::RemoveUser(User& user)
{
    _userMap.erase(&user);
    for ( UserList::iterator i = _userList.begin(); i != _userList.end(); i++ ) {
        if ( *i == &user ) {
            _userList.erase(i);
            break;
        }
    }
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

void ServerWorldInstance::Update(bool elapsed)
{
    WorldInstance::Update(elapsed);
    _updatesOffset += _userList.size() * 10.0 / FrameTimer::current().framerate();
    int updates = int(_updatesOffset);
    _updatesOffset -= updates;
    for ( int i = 0; i < updates; i++ ) {
        User* user = _userList.front();
        _userList.pop_front();
        user->sendNetworkUpdate(_userMap[user]);
        _userList.push_back(user);
    }
}