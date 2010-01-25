#ifndef NETWORK_CLIENT_H_
#define NETWORK_CLIENT_H_

#include <boost/noncopyable.hpp>
#include <RakNet/RakNetTypes.h>
#include "networkPacket.h"
#include "networkPacketManager.h"

class RakPeerInterface;

// Main client network controller class.
class NetworkClient: boost::noncopyable {
public:
    NetworkClient();
    ~NetworkClient() { disconnect(); }
    
    // Connect to the specified host.
	// host: IP or domain name
	// port: port the host is listening on
	// returns true to indicate that the connection process will proceed
	//         false if it cannot connect to the network
	bool connect(const std::string& host, int port);
    
    // If connected, disconnect from the server.
    void disconnect();

	// The reason we disconnected from the server.
	std::string disconnectReason() const;
    
    // True if the connection is active, else false.
    bool isActive() const { return (_peer != 0); }
	
	// True if we have connected with the host.
	bool isConnected() const { return _connected; }
	
	// Send out a packet to the server.
	// packet: the packet to send
	void send(const NetworkPacket& packet);
	
	// Poll to see if we've received anything; call in a loop.
	// precondition: isActive() must be true
	// returns a packet if one was received
	//         null if nothing's ready
	// Ownership of the packet passes to the caller.
	std::auto_ptr<NetworkPacket> receive();
	
	// The global server object.
	static NetworkClient& current() { assert(_current); return *_current; }
	static void setCurrent(NetworkClient *current) { _current = current; }

private:
    
	// The address of the server
	SystemAddress _address;

	// Factory which creates packets by packet ID.
	NetworkPacketManager _manager;

    // RakNet interface.
    RakPeerInterface* _peer;
	
	// Are we connected with the host?
	bool _connected;

	static NetworkClient *_current;
};

#endif
