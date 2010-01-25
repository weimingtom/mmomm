#ifndef NETWORK_SERVER_H_
#define NETWORK_SERVER_H_

#include <boost/noncopyable.hpp>
#include <boost/foreach.hpp>
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
		SystemAddress destination);
	
	// Send a packet to each of a sequence of desintations.
	// packet: the packet to send
	// begin/end: the iterators specifying who to send to
	template<typename ForwardIterator>
	void send(const NetworkPacket& packet,
		ForwardIterator begin, ForwardIterator end);
	
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

	// Send an encoded bitstream out.
	void rawSend(const BitStream& bs, const NetworkPacket& packet,
		SystemAddress address);
    
    // RakNet interface.
    RakPeerInterface *_peer;
	
	// Factory which creates packets by packet ID.
	NetworkPacketManager _manager;

	static NetworkServer *_current;
};


inline void NetworkServer::send(const NetworkPacket& packet,
	SystemAddress destination)
{
	BitStream bs;
	packet.write(bs);
	rawSend(bs, packet, destination);
}

template<typename ForwardIterator>
void NetworkServer::send(const NetworkPacket& packet,
	ForwardIterator begin, ForwardIterator end)
{
	BitStream bs;
	packet.write(bs);
	BOOST_FOREACH(SystemAddress& destination, std::make_pair(begin, end))
	{
		rawSend(bs, packet, destination);
	}
}

#endif
