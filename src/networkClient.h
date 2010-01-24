#ifndef NETWORK_CLIENT_H_
#define NETWORK_CLIENT_H_

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
	
	// Poll to see if we've received anything; call in a loop.
	// precondition: isActive() must be true
	// packet: where to store the packet data
	// returns true if a packet was received
	//         false if nothing's ready or if we disconnected
	//         if we disconnected, isActive will return false and
	//         disconnectReason will have the reason we disconnected
	bool Receive(NetworkPacket& packet);
    
private:
    
    // RakNet interface.
    RakPeerInterface* _peer;
	
	// Are we connected with the host?
	bool _connected;
};

#endif
