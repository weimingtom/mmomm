#ifndef NATIVE_PACKETS_H_
#define NATIVE_PACKETS_H_

// Packets sent by RakNet itself
// Do not register these packets; automatically generated

#include "networkPacket.h"
#include <RakNet/MessageIdentifiers.h>

// Occurs when a connection has been established.
class ConnectionPacket: public NetworkPacket {
public:

	NetworkParams params() const
	{
		return NetworkParams(ID_CONNECTION_REQUEST_ACCEPTED);
	}
	
	// Respond to successful connection on client.
	void respondClient() const;

	// Respond to a new incoming connection.
	void respondServer() const;
};

// Occurs when the server disconnects.
class DisconnectionPacket: public NetworkPacket {
public:
	
	DisconnectionPacket(const std::string& reason): _reason(reason) { }
	
	NetworkParams params() const
	{
		return NetworkParams(ID_DISCONNECTION_NOTIFICATION);
	}

	// Respond to being disconnected from the server, or the connection failing.
	void respondClient() const;

	// Respond to a new incoming connection.
	void respondServer() const;
	
	// The reason the connection has failed.
	std::string reason() const { return _reason; }
	
private:
	std::string _reason;
};

// Someone has been messing with packets (or programmer error).
class TamperPacket: public NetworkPacket {
public:

	NetworkParams params() const
	{
		return NetworkParams(ID_MODIFIED_PACKET);
	}

	// The server sent us a packet that's been tampered with.
	void respondClient() const;

	// The client's sent up a packet that's been tampered with.
	void respondServer() const;
};

#endif
