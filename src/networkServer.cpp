#include "networkServer.h"
#include "networkPacket.h"
#include "initializePacketManager.h"
#include "nativePackets.h"
#include <RakNet/RakNetTypes.h>
#include <RakNet/RakNetworkFactory.h>
#include <RakNet/RakPeerInterface.h>
#include <RakNet/MessageIdentifiers.h>
#include <iostream>

NetworkServer *NetworkServer::_current = 0;

NetworkServer::NetworkServer()
:	_peer()
,	_manager()
{
	initializePacketManager(_manager);
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

void NetworkServer::rawSend(const BitStream& bs,
	const NetworkPacket& packet, SystemAddress destination)
{
	assert(isConnected());

	// HACK: reinterpret due to weird API changes; should be kosher
	const char *data = reinterpret_cast<char *>(bs.GetData());
	bool success = _peer->Send(data, bs.GetNumberOfBytesUsed(),
		packet.priority(), packet.reliability(), packet.orderingChannel(),
		destination, false);
	(void)success;
	assert(success);
}

// RAII class that auto-cleans packets
class AutoDepacketer {
public:
	AutoDepacketer(RakPeerInterface *peer, Packet* packet):
		_peer(peer), _packet(packet) { }
	~AutoDepacketer() { _peer->DeallocatePacket(_packet); }

private:
    RakPeerInterface *_peer;
	Packet* _packet;
};

std::auto_ptr<NetworkPacket> NetworkServer::receive()
{
	assert(isConnected());

	Packet* raw = _peer->Receive();
	AutoDepacketer depacketer(_peer, raw);
	if (raw) {
		unsigned char kind = raw->data[0];
		if (kind == ID_TIMESTAMP) {
			kind = raw->data[1 + sizeof(RakNetTime)];
		}
		std::auto_ptr<NetworkPacket> packet;
		if (kind >= ID_USER_PACKET_ENUM) {
			packet.reset(_manager.create(kind).release());
			if (!packet.get()) {
				std::cout << "Unknown user packet detected: " << kind << std::endl;
				packet.reset(new TamperPacket());
			}
		}
		else {
			switch (kind) {
			case ID_NEW_INCOMING_CONNECTION:
				// TODO: Add client
				packet.reset(new ConnectionPacket());
				break;
			case ID_DISCONNECTION_NOTIFICATION:
			case ID_CONNECTION_LOST:
			{
				// TODO: Remove client
				std::string reason;
				switch (kind) {
				case ID_DISCONNECTION_NOTIFICATION:
					reason = "The client closed the connection.";
					break;
				default:
					reason = "The connection to the client was lost.";
				}
				packet.reset(new DisconnectionPacket(reason));
				break;
			}
			case ID_MODIFIED_PACKET:
				packet.reset(new TamperPacket());
				break;
			default:
				std::cout << "System packet ignored: " << kind << std::endl;
				return packet;
			}
		}

		assert(packet.get());
		packet->read(raw);
		return packet;
	}
	return std::auto_ptr<NetworkPacket>();
}

