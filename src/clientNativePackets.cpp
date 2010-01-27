#include "nativePackets.h"
#include <iostream>

void ConnectionPacket::respondClient() const
{
	std::cout << "connected" << std::endl;
}

void DisconnectionPacket::respondClient() const
{
	std::cout << "disconnected" << std::endl;
	
}

void TamperPacket::respondClient() const
{
	
	std::cout << "tampered" << std::endl;
}
