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
	// precondition: isActive() is false
	// host: IP or domain name
	// port: port the host is listening on
	// username: the name of the user to sign in as
	// password: the password corresponding to the username
	// createAccount: if true, create a new account, else log in to an existing one
	// returns true to indicate that the connection process will proceed
	//         false if it cannot connect to the network
	bool connect(const std::string& host, int port,
		const std::string& username, const std::string& password, bool createAccount);
    
    // If active, disconnect from the server.
    void disconnect();
	
    // True if the connection is active, else false.
	// Starts being true when connect() returns true
	// Stops being true when a DisconnectionPacket is received
    bool isActive() const { return (_state != STATE_DISCONNECTED); }
	
	// True if we have connected with the host.
	// Starts being true when a ConnectionPacket is received
	// Stops being true when a DisconnectionPacket is received
	bool isConnected() const { return (_state == STATE_CONNECTED); }

	// Send out a packet to the server.
	// precondition: isConnected() is true
	// packet: the packet to send
	void send(const NetworkPacket& packet);
	
	// Poll to see if we've received anything; call in a loop.
	// precondition: isActive() must be true
	// returns a packet if one was received
	//         null if nothing's ready
	// Ownership of the packet passes to the caller.
	typedef std::auto_ptr<NetworkPacket> AutoPacket;
	AutoPacket receive();
	
	// Gets the username logged in (or logging in) as
	std::string username() const { assert(isActive()); return _username; }
	
	// The address of the host.
	SystemAddress host() const { assert(isConnected()); return _address; }

	// The global server object.
	static NetworkClient& current() { assert(_current); return *_current; }
	static void setCurrent(NetworkClient *current) { _current = current; }

private:

	// Helper function for receiving a single possibly-internal packet.
	AutoPacket processPacket(const Packet& packet);
    
	enum ConnectionState {
		STATE_DISCONNECTED,
		STATE_CONNECTING,
		STATE_AUTHENTICATING,
		STATE_CONNECTED
	};
	
	// The address of the server
	SystemAddress _address;

	// Factory which creates packets by packet ID.
	NetworkPacketManager _manager;
	
    // RakNet interface.
    RakPeerInterface* _peer;
	
	// The login credentials.
	std::string _username;
	std::string _password;
	bool _createAccount;

	// Are we connected with the host?
	ConnectionState _state;

	// Packet queueing for assumption of connection.
	AutoPacket _queuedPacket;

	static NetworkClient *_current;
};

#endif
