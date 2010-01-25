
#include "networkServer.h"
#include <RakNet/RakSleep.h>
#include <iostream>

int main(int argc, char **argv)
{
	NetworkServer::setCurrent(new NetworkServer());
	
	int port = NetworkServer::DEFAULT_PORT;
	std::cout << "Listening on " << port << "..." << std::endl;
	NetworkServer::current().listen(port);
	
	// Keep looping until quit with control-c
	for (;;) {
		
		// Check packets
		for (;;) {
			// Check packets until we run out
			std::auto_ptr<NetworkPacket> packet = NetworkServer::current().receive();
			if (!packet.get()) break;
			
			packet->respondServer();
		}

		// Update time step.
		//world->update(time);
		
		// Sleep until next frame.
		RakSleep(30);
	}
}
