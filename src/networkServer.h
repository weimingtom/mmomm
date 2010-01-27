#ifndef NETWORK_SERVER_H_
#define NETWORK_SERVER_H_

#include <boost/noncopyable.hpp>
#include <boost/foreach.hpp>
#include <RakNet/RakNetTypes.h>
#include "networkPacket.h"
#include "networkPacketManager.h"
#include <map>
#include <vector>

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
	// precondition: isConnected() must be false
	// port: the port to listen on
	// maxConnections: the maximum number of clients allowed to connect
	// returns false if it cannot access the network
    bool listen(int port = DEFAULT_PORT, int maxConnections = DEFAULT_MAX_CONNECTIONS);
    
    // If connected, disconnect from all clients.
    void disconnect();
    
    // True if the connection is active, else false.
    bool isConnected() const { return (_peer != 0); }
    
	// Send out a packet to a specific user.
	// precondition: isConnected() must be true
	// packet: the packet to send
	// user: the user to send to
	void send(const NetworkPacket& packet,
		const User& user);
	
	// Send a packet to each of a sequence of users.
	// precondition: isConnected() must be true
	// packet: the packet to send
	// begin/end: iterators specifying who to send to
	template<typename ForwardIterator>
	void send(const NetworkPacket& packet,
		ForwardIterator begin, ForwardIterator end);
	
	// Poll to see if we've received anything; call in a loop.
	// precondition: isConnected() must be true
	// returns a packet if one was received
	//         null if nothing's ready
	// Ownership of the packet passes to the caller.
	typedef std::auto_ptr<NetworkPacket> AutoPacket;
	AutoPacket receive();
	
	// Gets the list of users.
	std::vector<User *> users() const;

	// Gets a specific user by name
	// returns null if not found
	User *getUser(const std::string& name) const;
	
	// The global server object.
	static NetworkServer& current() { assert(_current); return *_current; }
	static void setCurrent(NetworkServer *current) { _current = current; }
	
	// Read/write the authentication file
	// TODO: Authentication file serialization.
	void readAuthenticationFile(const std::string& filename) { (void)filename; }
	void writeAuthenticationFile(const std::string& filename) { (void)filename; }

private:
	
	// Helper function for receiving a single possibly-internal packet.
	AutoPacket processPacket(const Packet& packet);
	
	// Send an encoded bitstream out.
	void rawSend(const BitStream& bs, const NetworkPacket& packet,
		const User& user);
    
	// Creates a new user from the parameters specified in the packet.
	// Returns null if the user cannot be created.
	User *createUser(const Packet& packet);
	
	// Removes and destroys the specified user from the user list.
	void removeUser(User *user);

	// Removes all the users in the list.
	void clearUsers();
	
	// Finds the user corresponding to the given address, if any.
	User *findUser(const SystemAddress& address) const;
	
	// Create a salty password.
	// TODO: Hash passwords.
	static std::string hashPassword(const std::string& username, const std::string& password) { (void)username; return password; }

	// Cleans a username and returns the result.
	static std::string cleanName(const std::string username);
	
	// The mapping of usernames to hashed passwords
	typedef std::map<std::string, std::string> AuthenticationMap;
	AuthenticationMap _authMap;
	
	// The mapping of addresses to users.
	typedef std::map<SystemAddress, User *> UserMap;
	UserMap _userMap;
	
    // RakNet interface.
    RakPeerInterface *_peer;
	
	// Factory which creates packets by packet ID.
	NetworkPacketManager _manager;

	// The user to remove on the next receive.
	User *_queuedUser;
	
	static NetworkServer *_current;
};


inline void NetworkServer::send(const NetworkPacket& packet,
	const User& user)
{
	BitStream bs;
	packet.write(bs);
	rawSend(bs, packet, user);
}

template<typename ForwardIterator>
void NetworkServer::send(const NetworkPacket& packet,
	ForwardIterator begin, ForwardIterator end)
{
	BitStream bs;
	packet.write(bs);
	BOOST_FOREACH(const User *user, std::make_pair(begin, end))
	{
		rawSend(bs, packet, *user);
	}
}

#endif
