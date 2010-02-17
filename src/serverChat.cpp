#include "chat.h"
#include "user.h"
#include "networkServer.h"
#include <iostream>

void ChatMessagePacket::respondServer() const
{
	ChatMessagePacket packet(sender().username(), target(), message(), type());
	std::vector<User *> users = NetworkServer::current().users();
	if(type() == CHAT_MESSAGE_PRIVATE) {
        BOOST_FOREACH(User *user, users) {
            if (user->username() == target()) {
                NetworkServer::current().send(packet, *user);
            }
            else if(user->username() == from()) {
                NetworkServer::current().send(packet, *user);
            }
        }
	}
	else {
        NetworkServer::current().send(packet, users.begin(), users.end());
	}
}

