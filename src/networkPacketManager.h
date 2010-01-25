#ifndef NETWORK_PACKET_MANAGER_H_
#define NETWORK_PACKET_MANAGER_H_

#include "networkPacket.h"
#include <map>
#include <boost/function.hpp>
#include <boost/tuple/tuple.hpp>

// Provides registration for network packet creation and calling.
class NetworkPacketManager {
public:
	// The type of function that produces a new kind of packet.
	typedef boost::function<std::auto_ptr<NetworkPacket>()> CreationFunc;

	// Connects a kind ID with the specified packet type.
	// kind: the corresponding kind enum
	// creation: the function that creates a new packet
	// response: the function that responds to a received packet of this type
	void assign(unsigned char kind,
		const CreationFunc& creation);
	
	// A simple creation function you can use.
	template<typename PacketType>
	static std::auto_ptr<NetworkPacket> creation();
	
	// Creates and initializes a packet of the specified kind
	std::auto_ptr<NetworkPacket> create(unsigned char kind) const;

private:
	typedef std::map<unsigned char, CreationFunc> ManagerMap;
	ManagerMap _managerMap;
};

template<typename PacketType>
inline std::auto_ptr<NetworkPacket> NetworkPacketManager::creation()
{
	return std::auto_ptr<NetworkPacket>(new PacketType());
}

inline std::auto_ptr<NetworkPacket> NetworkPacketManager::create(unsigned char kind) const
{
	ManagerMap::const_iterator found = _managerMap.find(kind);
	if (found != _managerMap.end()) {
		return found->second();
	}
	return std::auto_ptr<NetworkPacket>();
}

inline void NetworkPacketManager::assign(unsigned char kind,
		const CreationFunc& creation) {
	_managerMap[kind] = creation;
}

#endif

