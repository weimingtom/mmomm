#include "networkServer.h"
#include "networkPacket.h"

#include <RakNet/RakNetTypes.h>
#include <RakNet/RakNetworkFactory.h>
#include <RakNet/RakPeerInterface.h>
#include <RakNet/MessageIdentifiers.h>

NetworkServer::NetworkServer()
:	_peer()
{
}

bool NetworkServer::listen(int port, int maxConnections)
{
    assert(!isConnected());
    assert(maxConnections > 0);
    assert(port >= 0 && port <= 65535);
    
	RakPeerInterface* peer = RakNetworkFactory::GetRakPeerInterface();

	int threadSleepMillis = 30;
	SocketDescriptor descriptor(port, 0);
	if (!peer->Startup(maxConnections, threadSleepMillis, &descriptor, 1)) {
		RakNetworkFactory::DestroyRakPeerInterface(peer);
		return false;
	}
    peer->SetMaximumIncomingConnections(maxConnections);
	peer->SetOccasionalPing(true);
	
	_peer = peer;
	return true;
}

void NetworkServer::disconnect()
{
    if (isConnected()) {
        // TODO: Kick all clients.
        
		// Wait for 1 seconds to try to get everything out
        _peer->Shutdown(1000);
        RakNetworkFactory::DestroyRakPeerInterface(_peer);
        _peer = 0;
    }
    
}

void NetworkServer::rawSend(const NetworkPacket& packet,
	SystemAddress destination,
	bool broadcast)
{
	BitStream bs;
	packet.write(bs);
	// HACK: reinterpret due to weird API changes; should be kosher
	const char *data = reinterpret_cast<char *>(bs.GetData());
	bool success = _peer->Send(data, bs.GetNumberOfBytesUsed(),
		packet.priority(), packet.reliability(), packet.orderingChannel(),
		destination, broadcast);
}

std::auto_ptr<NetworkPacket> NetworkServer::receive()
{
	assert(isConnected());

	Packet* raw = _peer->Receive();
	if (raw) {
		unsigned char kind = raw->data[0];
		if (kind == ID_TIMESTAMP) {
			kind = raw->data[1 + sizeof(RakNetTime)];
		}
		std::auto_ptr<NetworkPacket> packet(_factory.create(kind));
		if (packet.get()) {
			packet->read(raw);
		}
		_peer->DeallocatePacket(raw);
		return packet;
	}
	return std::auto_ptr<NetworkPacket>();
}

