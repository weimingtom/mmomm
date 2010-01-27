#include "nativePackets.h"
#include "user.h"
#include <iostream>

void ConnectionPacket::respondServer() const
{
	std::cout << "connected " << sender().username() << std::endl;
}

void DisconnectionPacket::respondServer() const
{
	std::cout << "disconnected " << sender().username() << std::endl;
	
}

void TamperPacket::respondServer() const
{
	std::cout << "tamper " << sender().username() << std::endl;
	
}
