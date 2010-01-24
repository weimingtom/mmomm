#ifndef NETWORK_PACKET_FACTORY_H_
#define NETWORK_PACKET_FACTORY_H_

#include "networkPacket.h"
#include <map>
#include <boost/function.hpp>

// Provides registration for network packet creation.
class NetworkPacketFactory {
public:
	// Connects a kind ID with the specified packet type.
	// PacketType: the type of packet to create; must be default constructable
	// kind: the corresponding kind enum
	template<typename PacketType>
	void assign(unsigned char kind);
	
	// Creates and initializes a packet of the specified kind
	std::auto_ptr<NetworkPacket> create(unsigned char kind) const;

private:
	typedef boost::function<std::auto_ptr<NetworkPacket>()> CreationFunc;
	typedef std::map<unsigned char, CreationFunc> CreationMap;
	CreationMap _createMap;
};

inline std::auto_ptr<NetworkPacket> NetworkPacketFactory::create(unsigned char kind) const
{
	CreationMap::const_iterator found = _createMap.find(kind);
	if (found == _createMap.end()) {
		return std::auto_ptr<NetworkPacket>();
	}
	return found->second();
}

template<typename PacketType>
NetworkPacket *createPacket()
{
	return std::auto_ptr<NetworkPacket>(new PacketType());
}

template<typename PacketType>
void NetworkPacketFactory::assign(unsigned char kind) {
	_createMap[kind] = createPacket<PacketType>;
}

#endif

