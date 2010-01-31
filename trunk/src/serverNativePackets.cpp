#include "nativePackets.h"
#include "user.h"
#include "playerActor.h"
#include "serverWorldInstance.h"
#include <iostream>

void ConnectionPacket::respondServer() const
{
	std::cout << "connected " << sender().username() << std::endl;
    PlayerActor* actor = new PlayerActor(sender(), Rect(0.0, 0.0, 1.0, 1.0));
    ServerWorldInstance::current().AddUser(sender(), actor);
}

void DisconnectionPacket::respondServer() const
{
	std::cout << "disconnected " << sender().username() << std::endl;
    delete ServerWorldInstance::current().GetUserActor(sender());
    ServerWorldInstance::current().RemoveUser(sender());
}

void TamperPacket::respondServer() const
{
	std::cout << "tamper " << sender().username() << std::endl;
	
}
