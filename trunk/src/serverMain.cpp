#include "serverWorldInstance.h"
#include "networkServer.h"
#include "frameTimer.h"
#include <iostream>

#include "collisionPackets.h"

int main(int argc, char **argv)
{
	
	NetworkServer::setCurrent(new NetworkServer());
    WorldInstance::setCurrent(new ServerWorldInstance());
	int port = NetworkServer::DEFAULT_PORT;
	if (!NetworkServer::current().listen(port)) {
		std::cout << "Failed to access network." << std::endl;
		return 1;
	}
	
	std::cout << "Listening on " << port << "..." << std::endl;

	FrameTimer::setCurrent(new FrameTimer());
	
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
		FrameTimer::current().step();
	}

    delete &NetworkServer::current();
    delete &WorldInstance::current();
}
