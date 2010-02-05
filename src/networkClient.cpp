#include "networkClient.h"
#include "networkCommon.h"
#include "networkPacket.h"
#include "initializePacketManager.h"
#include "nativePackets.h"
#include "packetTypes.h"
#include "serial.h"
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
,	_username()
,	_password()
,	_createAccount(false)
,	_state(STATE_DISCONNECTED)
,	_queuedPacket()
{
	initializePacketManager(_manager);
}

bool NetworkClient::connect(const std::string& host, int port,
							const std::string& username, const std::string& password,
							bool createAccount)
{
    assert(!isActive());
    assert(port >= 0 && port <= 65535);
    
	RakPeerInterface *peer = RakNetworkFactory::GetRakPeerInterface();
	
	int threadSleepMillis = 30;
	SocketDescriptor descriptor(0, 0);
	if (peer->Startup(1, threadSleepMillis, &descriptor, 1)) {
		
		unsigned short shrunkenPort = static_cast<unsigned short>(port);
		if (peer->Connect(host.c_str(), shrunkenPort, 0, 0)) {
			peer->SetOccasionalPing(true);
			_state = STATE_CONNECTING;
			_username = username;
			_password = password;
			_createAccount = createAccount;
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
    if (isActive()) {
		// Wait for 1 second to try to get everything out
        _peer->Shutdown(1000);
        RakNetworkFactory::DestroyRakPeerInterface(_peer);
        _peer = 0;
		_state = STATE_DISCONNECTED;
		_password = "";
		_queuedPacket.reset();
    }
}

void NetworkClient::send(const NetworkPacket& packet)
{
	assert(isConnected());
	
	BitStream bs;
	packet.write(bs);
	
	sendBitStream(*_peer, bs, packet.params(), _address);
}

NetworkClient::AutoPacket NetworkClient::receive()
{
	assert(isActive());

	if (_queuedPacket.get()) {
		return _queuedPacket;
	}
	
	for (;;) {
		Packet* raw = _peer->Receive();
		if (!raw) {
			return AutoPacket();
		}
		AutoDepacketer depacketer(*_peer, raw);
		
		AutoPacket packet(processPacket(*raw));
		
		if (!_peer) {
			// Only disconnection packets should disconnect
			assert(packet.get());
			assert(packet->kind() == ID_ACCOUNT_FAILURE);
			depacketer.reset();
			return packet;
		}
		
		if (!packet.get()) {
			continue;
		}
		
		// Only disconnect and connect packets are allowed when not connected
		if (!isConnected() &&
				packet->kind() != ID_ACCOUNT_SUCCESS &&
				packet->kind() != ID_ACCOUNT_FAILURE) {
			
			if (_state != STATE_AUTHENTICATING) {
				packetHeader(std::cout, *raw);
				std::cout << "Received packet before being connected: " << packet->kind() << std::endl;
				continue;
			}
			// Assume a connection; packet just didn't get here first
			_state = STATE_CONNECTED;
			packet->read(raw);
			_queuedPacket.reset(packet.release());
			packet.reset(new ConnectionPacket());

			packetHeader(std::cout, *raw);
			std::cout << "Received packet while authenticating (early connect): " << packet->kind() << std::endl;
		}

		assert(packet.get());
		packet->read(raw);
		return packet;
	}
}

NetworkClient::AutoPacket NetworkClient::processPacket(const Packet& raw)
{
	unsigned char kind = packetKind(raw);
	
	// User packets
	if (kind >= ID_NOT_INTERNAL) {
		AutoPacket packet(_manager.create(kind));
		if (packet.get()) {
			return packet;
		}
		packetHeader(std::cout, raw);
		std::cout << "Unknown user packet detected: " << kind << std::endl;
		return AutoPacket(new TamperPacket);
	}
	
	switch (kind) {
	// The server just allowed us in; time to log on
	case ID_CONNECTION_REQUEST_ACCEPTED:
	{
		if (_state != STATE_CONNECTING) {
			packetHeader(std::cout, raw);
			std::cout << "Duplicate connection accepted: " << kind << std::endl;
			return AutoPacket();
		}
		_state = STATE_AUTHENTICATING;
		_address = raw.systemAddress;
		
		BitStream bs;
		bs.Write(static_cast<uint8_t>(ID_ACCOUNT_LOGIN));
		serial(bs, true, _username);
		serial(bs, true, _password);
		bs.Write(_createAccount);
		NetworkParams params(ID_ACCOUNT_LOGIN, HIGH_PRIORITY);
		sendBitStream(*_peer, bs, params, _address);
		
		// Don't keep password in memory (imperfect but better than nothing)
		_password = "";
		
		// Still connecting, so don't tell the client yet
		return AutoPacket();
	}
	// The server won't let us in.
	case ID_CONNECTION_ATTEMPT_FAILED:
	case ID_ALREADY_CONNECTED: // Shouldn't happen, but may.
	case ID_IP_RECENTLY_CONNECTED:
	case ID_NO_FREE_INCOMING_CONNECTIONS:
	case ID_CONNECTION_BANNED:
	{
		if (isConnected()) {
			packetHeader(std::cout, raw);
			std::cout << "Connection attempt failed while active; ignored. " << kind << std::endl;
			return AutoPacket();
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
		return AutoPacket(new DisconnectionPacket(reason));
	}
	// Lost connection to server.
	case ID_DISCONNECTION_NOTIFICATION: // shut down or kick
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
		return AutoPacket(new DisconnectionPacket(reason));
	}
	// We successfully authenticated
	case ID_ACCOUNT_SUCCESS:
	{
		if (_state != STATE_AUTHENTICATING) {
			packetHeader(std::cout, raw);
			std::cout << "Duplicate authentication ignored: " << kind << std::endl;
			return AutoPacket();
		}
		_state = STATE_CONNECTED;
		
		// Alert the client that we are connected.
		return AutoPacket(new ConnectionPacket());
	}
	// We failed authentication
	case ID_ACCOUNT_FAILURE:
	{
		if (_state != STATE_AUTHENTICATING) {
			packetHeader(std::cout, raw);
			std::cout << "Duplicate authentication ignored: " << kind << std::endl;
			return AutoPacket();
		}
		std::string reason;
		if (_createAccount) {
			reason = "The specified username is already taken.";
		}
		else {
			reason = "Your username or password is incorrect or already logged in.";
		}
		return AutoPacket(new DisconnectionPacket(reason));
	}
	// A packet was tampered with in transit
	case ID_MODIFIED_PACKET:
		return AutoPacket(new TamperPacket());
	// Some other packet we don't care about
	default:
		packetHeader(std::cout, raw);
		std::cout << "Unused system packet ignored: " << kind << std::endl;
		return AutoPacket();
	}
}
