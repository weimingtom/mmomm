#include "worldInstance.h"
#include "networkServer.h"
#include <RakNet/RakSleep.h>
#include <iostream>

int main(int argc, char **argv)
{
	NetworkServer::setCurrent(new NetworkServer());
    WorldInstance::setCurrent(new WorldInstance());
	
	int port = NetworkServer::DEFAULT_PORT;
	if (!NetworkServer::current().listen(port)) {
		std::cout << "Failed to access network." << std::endl;
		return 1;
	}
	
	std::cout << "Listening on " << port << "..." << std::endl;

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
        WorldInstance::current().Update();
		
		// Sleep until next frame.
		RakSleep(30);
	}

    delete &NetworkServer::current();
    delete &WorldInstance::current();
}
