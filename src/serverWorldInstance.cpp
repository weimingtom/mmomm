#include "serverWorldInstance.h"
#include <iostream>
#include <boost/foreach.hpp>
#include "frameTimer.h"
#include "playerActor.h"
#include "serverWorldMap.h"
#include "user.h"

ServerWorldInstance::ServerWorldInstance()
: WorldInstance(new TestCollision())
, _updatesOffset(0)
, _worldMap(new ServerWorldMap())
{
}

ServerWorldInstance::~ServerWorldInstance()
{
	delete _worldMap;
}

void ServerWorldInstance::addUser(User& user, PlayerActor* actor)
{
    _userMap[&user] = actor;
    _userList.push_front(&user);
}

void ServerWorldInstance::removeUser(User& user)
{
    _userMap.erase(&user);
    for ( UserList::iterator i = _userList.begin(); i != _userList.end(); i++ ) {
        if ( *i == &user ) {
            _userList.erase(i);
            break;
        }
    }
}

PlayerActor* ServerWorldInstance::getUserActor(User& user) const
{
    UserMap::const_iterator i = _userMap.find(&user);
    if ( i == _userMap.end() )
        return 0;
    return i->second;
}

const ServerWorldInstance::UserMap& ServerWorldInstance::getUserMap() const
{
    return _userMap;
}

ServerWorldInstance& ServerWorldInstance::current()
{
    assert(dynamic_cast< ServerWorldInstance* >(&WorldInstance::current()));
    return *(ServerWorldInstance*)&WorldInstance::current();
}

void ServerWorldInstance::update(double elapsed)
{
    WorldInstance::update(elapsed);

	// Update tiles for each user
	BOOST_FOREACH(User *user, _userList) {
		user->sendWorldMapUpdate(_userMap[user]);
	}

	// Update actors for each user
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