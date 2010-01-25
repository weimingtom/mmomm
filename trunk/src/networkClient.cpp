#include "networkClient.h"
#include "networkPacket.h"
#include "initializePacketManager.h"
#include "nativePackets.h"
#include <RakNet/RakNetTypes.h>
#include <RakNet/RakNetworkFactory.h>
#include <RakNet/RakPeerInterface.h>
#include <RakNet/MessageIdentifiers.h>
#include <iostream>

NetworkClient *NetworkClient::_current = 0;

NetworkClient::NetworkClient()
:	_address(UNASSIGNED_SYSTEM_ADDRESS)
,	_manager()
,	_peer()
,	_connected(false)
{
	initializePacketManager(_manager);
}

bool NetworkClient::connect(const std::string& host, int port)
{
    assert(!isConnected());
    assert(port >= 0 && port <= 65535);
    
	RakPeerInterface *peer = RakNetworkFactory::GetRakPeerInterface();

	int threadSleepMillis = 30;
	SocketDescriptor descriptor(0, 0);
	if (peer->Startup(1, threadSleepMillis, &descriptor, 1)) {
		
		unsigned short shrunkenPort = static_cast<unsigned short>(port);
		if (peer->Connect(host.c_str(), shrunkenPort, 0, 0)) {
			peer->SetOccasionalPing(true);
			_peer = peer;
			return true;
		}
		
		// Probably nothing to send; don't need to wait very long.
		peer->Shutdown(100);
	}
	RakNetworkFactory::DestroyRakPeerInterface(peer);
	return false;
}

void NetworkClient::disconnect()
{
    if (isConnected()) {
		// Wait for 1 second to try to get everything out
        _peer->Shutdown(1000);
        RakNetworkFactory::DestroyRakPeerInterface(_peer);
        _peer = 0;
    }
}

void NetworkClient::send(const NetworkPacket& packet)
{
	assert(isConnected());
	
	BitStream bs;
	packet.write(bs);
	// HACK: reinterpret due to weird API changes; should be kosher
	const char *data = reinterpret_cast<char *>(bs.GetData());
	bool success = _peer->Send(data, bs.GetNumberOfBytesUsed(),
		packet.priority(), packet.reliability(), packet.orderingChannel(),
		_address, false);
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

std::auto_ptr<NetworkPacket> NetworkClient::receive()
{
	assert(isActive());
	
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
			case ID_CONNECTION_REQUEST_ACCEPTED:
				if (isConnected()) {
					std::cout << "Duplicate connection request." << std::endl;
					return packet;
				}
				packet.reset(new ConnectionPacket());
				_connected = true;
				_address = raw->systemAddress;
				disconnect();
				break;
			case ID_CONNECTION_ATTEMPT_FAILED:
			case ID_ALREADY_CONNECTED: // Shouldn't happen, but may.
			case ID_IP_RECENTLY_CONNECTED:
			case ID_NO_FREE_INCOMING_CONNECTIONS:
			case ID_CONNECTION_BANNED:
			{
				if (isConnected()) {
					std::cout << "Connection attempt failed while active; ignored." << std::endl;
					return packet;
				}
				disconnect();
				std::string reason;
				switch (kind) {
				case ID_ALREADY_CONNECTED:
				case ID_IP_RECENTLY_CONNECTED:
					reason = "The server refused the connection because you have been connecting too frequently; wait a short time before trying again.";
					break;
				case ID_NO_FREE_INCOMING_CONNECTIONS:
					reason = "The server player limit has been reached.";
					break;
				case ID_CONNECTION_BANNED:
					reason = "Your IP has been banned from this server.";
					break;
				default:
					reason = "The server could not be found.";
				}
				packet.reset(new DisconnectionPacket(reason));
				break;
			}
			case ID_DISCONNECTION_NOTIFICATION:
			case ID_CONNECTION_LOST:
			{
				disconnect();
				std::string reason;
				switch (kind) {
				case ID_DISCONNECTION_NOTIFICATION:
					reason = "The server has shut down.";
					break;
				default:
					reason = "Your connection to the server has been lost.";
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
		
		if (!isConnected() &&
			kind != ID_CONNECTION_REQUEST_ACCEPTED &&
			kind != ID_DISCONNECTION_NOTIFICATION) {
			return std::auto_ptr<NetworkPacket>();
		}
		
		assert(packet.get());
		packet->read(raw);
		return packet;
	}
	return std::auto_ptr<NetworkPacket>();
}

