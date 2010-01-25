#ifndef NETWORK_SERVER_H_
#define NETWORK_SERVER_H_

#include <boost/noncopyable.hpp>
#include <RakNet/RakNetTypes.h>
#include "networkPacket.h"
#include "networkPacketManager.h"

class RakPeerInterface;
class NetworkPacket;

// Main server network controller class.
class NetworkServer: boost::noncopyable {
public:
	// Default maximum number of connections.
	static const int DEFAULT_MAX_CONNECTIONS = 20;

	// Default port number;
	static const int DEFAULT_PORT = 33033;
	
    NetworkServer();
    ~NetworkServer() { disconnect(); }
    
    // Listen for new incoming connections.
	// port: the port to listen on
	// maxConnections: the maximum number of clients allowed to connect
	// returns false if it cannot access the network
    bool listen(int port = DEFAULT_PORT, int maxConnections = DEFAULT_MAX_CONNECTIONS);
    
    // If connected, disconnect from all clients.
    void disconnect();
    
    // True if the connection is active, else false.
    bool isConnected() const { return (_peer != 0); }
    
	// Send out a packet to a specific user.
	// packet: the packet to send
	// destination: the address of the user to send to
	void send(const NetworkPacket& packet,
		SystemAddress destination) { rawSend(packet, destination, false); }
	
	// Send out a packet to all users.
	// packet: the packet to send
	// exclude: an optional single user to exclude sending to.
	void broadcast(const NetworkPacket& packet,
		SystemAddress exclude=UNASSIGNED_SYSTEM_ADDRESS)
		 { rawSend(packet, exclude, true); }
	
	// Poll to see if we've received anything; call in a loop.
	// precondition: isActive() must be true
	// returns a packet if one was received
	//         null if nothing's ready
	// Ownership of the packet passes to the caller.
	std::auto_ptr<NetworkPacket> receive();
	
	// The global server object.
	static NetworkServer& current() { assert(_current); return *_current; }
	static void setCurrent(NetworkServer *current) { _current = current; }

private:
    
	// Common implementation of the send functions.
	void rawSend(const NetworkPacket& packet,
		SystemAddress destination,
		bool broadcast);
	
    // RakNet interface.
    RakPeerInterface *_peer;
	
	// Factory which creates packets by packet ID.
	NetworkPacketManager _manager;

	static NetworkServer *_current;
};

#endif
