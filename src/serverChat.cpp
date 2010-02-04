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
            if(!user->username().compare(target())) { //compare returns 0 if equal
                NetworkServer::current().send(packet, *user);
            }
            else if(!user->username().compare(from())) { //compare returns 0 if equal
                NetworkServer::current().send(packet, *user);
            }
        }
	}
	else
        NetworkServer::current().send(packet, users.begin(), users.end());
}

