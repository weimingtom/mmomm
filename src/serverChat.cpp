#include "chat.h"
#include "user.h"
#include "networkServer.h"
#include <iostream>

void ChatMessagePacket::respondServer() const
{
	ChatMessagePacket packet(sender().username(), target(), message(), type());
	std::vector<User *> users = NetworkServer::current().users();
	NetworkServer::current().send(packet, users.begin(), users.end());
}

