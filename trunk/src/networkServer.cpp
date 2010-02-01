#include "networkServer.h"
#include "networkCommon.h"
#include "networkPacket.h"
#include "user.h"
#include "initializePacketManager.h"
#include "nativePackets.h"
#include <RakNet/RakNetTypes.h>
#include <RakNet/RakNetworkFactory.h>
#include <RakNet/RakPeerInterface.h>
#include <RakNet/MessageIdentifiers.h>
#include <iostream>
#include <cctype>
#include <functional>

const std::string NetworkServer::DEFAULT_HOST;

NetworkServer *NetworkServer::_current = 0;

char toLower(char c)
{
    return std::tolower(c);
}

NetworkServer::NetworkServer()
:	_authMap()
,	_userMap()
,	_peer()
,	_manager()
,	_queuedUser()
{
	initializePacketManager(_manager);

	// HACK: INTIIAL PASSWORDS
	_authMap["hangar"] = hashPassword("hangar", "awesome");
	_authMap["doctus"] = hashPassword("doctus", "evilgenius");
	_authMap["seiken"] = hashPassword("seiken", "bastard");
	_authMap["oipo"] = hashPassword("oipo", "garybusey");
	_authMap["garrick"] = hashPassword("garrick", "vampire");
}

bool NetworkServer::listen(const std::string& host, int port, int maxConnections)
{
    assert(!isConnected());
    assert(maxConnections > 0);
    assert(port >= 0 && port <= 65535);

	RakPeerInterface* peer = RakNetworkFactory::GetRakPeerInterface();

	int threadSleepMillis = 30;
	SocketDescriptor descriptor(port, host.c_str());
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
		clearUsers();
        _queuedUser = 0;

		// Wait for 1 second to try to get everything out
        _peer->Shutdown(1000);
        RakNetworkFactory::DestroyRakPeerInterface(_peer);
        _peer = 0;
    }
}

void NetworkServer::rawSend(const BitStream& bs,
	const NetworkPacket& packet, const User& user)
{
	assert(isConnected());
	sendBitStream(*_peer, bs, packet.params(), user.address());
}

NetworkServer::AutoPacket NetworkServer::receive()
{
	assert(isConnected());

	for (;;) {
		if (_queuedUser) {
			removeUser(_queuedUser);
			_queuedUser = 0;
		}

		Packet* raw = _peer->Receive();
		if (!raw) {
			return AutoPacket();
		}
		AutoDepacketer depacketer(*_peer, raw);

		AutoPacket packet(processPacket(*raw));
		if (packet.get()) {
			User *user = findUser(raw->systemAddress);
			if (!user) {
				packetHeader(std::cout, *raw);
				std::cout << "Packet from unknown user: " << packet->kind() << std::endl;
				continue;
			}
			packet->read(raw, user);
			return packet;
		}
	}
}

NetworkServer::AutoPacket NetworkServer::processPacket(const Packet& raw)
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
	// A client just connected for the first time
	case ID_NEW_INCOMING_CONNECTION:
	{
		// TODO: Timeout for no login
		return AutoPacket();
	}
	// A client is asking to login/register
	case ID_ACCOUNT_LOGIN:
	{
		const User *user = createUser(raw);
		unsigned char sendKind = ID_ACCOUNT_FAILURE;
		AutoPacket packet;
		if (user) {
			sendKind = ID_ACCOUNT_SUCCESS;
			packet.reset(new ConnectionPacket());
		}
		NetworkParams params(sendKind, HIGH_PRIORITY);
		sendSimplePacket(*_peer, params, raw.systemAddress);
		return packet;
	}
	// A client has disconnected
	case ID_DISCONNECTION_NOTIFICATION:
	case ID_CONNECTION_LOST:
	{
		User *user = findUser(raw.systemAddress);
		if (!user) {
			return AutoPacket();
		}
		assert(!_queuedUser);
		_queuedUser = user;

		std::string reason;
		switch (kind) {
		case ID_DISCONNECTION_NOTIFICATION:
			reason = "The client closed the connection.";
			break;
		default:
			reason = "The connection to the client was lost.";
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

std::vector<User *> NetworkServer::users() const
{
	std::vector<User *> list;
	BOOST_FOREACH(const UserMap::value_type& entry, _userMap) {
		list.push_back(entry.second);
	}
	return list;

}

User *NetworkServer::getUser(const std::string& name) const
{
	std::string clean = cleanName(name);
	BOOST_FOREACH(User *user, users()) {
		if (cleanName(user->username()) == clean) {
			return user;
		}
	}
	return 0;
}

std::string NetworkServer::cleanName(const std::string username)
{
	std::string lowerUsername = username;
	std::transform(lowerUsername.begin(), lowerUsername.end(),
		lowerUsername.begin(), toLower);
	return lowerUsername;
}

User *NetworkServer::createUser(const Packet& packet)
{
	std::auto_ptr<BitStream> bs(packetStream(packet));
	std::string username;
	std::string password;
	bool createAccount;

	serial(*bs, false, username);
	serial(*bs, false, password);
	bs->Read(createAccount);

	std::string lowerUsername = cleanName(username);
	password = hashPassword(lowerUsername, password);
	if (!createAccount) {
		AuthenticationMap::const_iterator found = _authMap.find(lowerUsername);
		if (found == _authMap.end() || found->second != password) {
			return 0;
		}
	}
	else {
		typedef std::pair<AuthenticationMap::iterator, bool> Result;
		Result slot = _authMap.insert(AuthenticationMap::value_type(lowerUsername, password));
		if (!slot.second) {
			return 0;
		}
	}
	typedef std::pair<UserMap::iterator, bool> Result;
	Result slot = _userMap.insert(UserMap::value_type(packet.systemAddress, 0));
	if (!slot.second) {
		return 0;
	}
	User *user = new User(username, packet.systemAddress);
	slot.first->second = user;
	return user;
}

void NetworkServer::removeUser(User *user)
{
	assert(user);
	assert(_userMap.find(user->address()) != _userMap.end());
	_userMap.erase(user->address());
	delete user;
}

void NetworkServer::clearUsers()
{
	BOOST_FOREACH(const UserMap::value_type& userPair, _userMap) {
		delete userPair.second;
	}
	_userMap.clear();
}

User *NetworkServer::findUser(const SystemAddress& address) const
{
	UserMap::const_iterator found = _userMap.find(address);
	if (found == _userMap.end()) {
		return 0;
	}
	return found->second;
}
