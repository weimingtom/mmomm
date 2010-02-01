#include "serverWorldInstance.h"
#include "networkServer.h"
#include "frameTimer.h"
#include <iostream>
#include <sstream>

int listen(int argc, char **argv)
{
	// Argument parsing
	if (argc > 3) {
		std::cout << "usage: " << argv[0] << " [[host]:port] [max connections]" << std::endl;
		return 1;
	}
	
	// host:port
	std::string host = NetworkServer::DEFAULT_HOST;
	int port = NetworkServer::DEFAULT_PORT;
	if (argc > 1) {
		std::string hostport = argv[1];
		int index = hostport.find(':');
		if (index > 0) {
			host = hostport.substr(0, index);
			hostport = hostport.substr(index + 1);
		}
		std::stringstream ss(hostport);
		port = 0;
		ss >> port;
		if (port < 1 || port > 65535) {
			std::cout << "port must be between 1 and 65535, got " << hostport << std::endl;
			return 1;
		}
	}

	int maxConnections = NetworkServer::DEFAULT_MAX_CONNECTIONS;
	if (argc > 2) {
		std::stringstream ss(argv[2]);
		maxConnections = 0;
		ss >> maxConnections;
		if (maxConnections < 1) {
			std::cout << "maximum number of connections must be greater than 0, got " << argv[2] << std::endl;
			return 1;
		}
	}

	
	if (!NetworkServer::current().listen(host, port, maxConnections)) {
		std::cout << "Failed to access network." << std::endl;
		return 1;
	}
	if (host.length()) {
		std::cout << "Listening on " << host << ":" << port << "..." << std::endl;
	}
	else {
		std::cout << "Listening on " << port << "..." << std::endl;
	}
	return 0;
}

int main(int argc, char **argv)
{
	NetworkServer::setCurrent(new NetworkServer());
    WorldInstance::setCurrent(new ServerWorldInstance());
	
	if (listen(argc, argv))
		return 1;

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
        WorldInstance::current().Update(FrameTimer::current().elapsed());
		
		// Sleep until next frame.
		FrameTimer::current().step();
	}

    delete &NetworkServer::current();
    delete &WorldInstance::current();
}
