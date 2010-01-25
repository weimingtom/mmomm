#include "networkCommon.h"
#include <RakNet/MessageIdentifiers.h>
#include <iostream>

void sendSimplePacket(RakPeerInterface& peer, const NetworkParams& params,
					  const SystemAddress& address)
{
	char kind = params.kind;
	bool success = peer.Send(&kind, 1,
		params.priority, params.reliability, params.orderingChannel,
		address, false);
	(void)success;
	assert(success);
}

void sendBitStream(RakPeerInterface& peer, const BitStream& bs,
				   const NetworkParams& params, const SystemAddress& address)
{
	// HACK: reinterpret due to weird API changes; should be kosher
	const char *data = reinterpret_cast<char *>(bs.GetData());
	bool success = peer.Send(data, bs.GetNumberOfBytesUsed(),
		params.priority, params.reliability, params.orderingChannel,
		address, false);
	(void)success;
	assert(success);
}

unsigned char packetKind(const Packet& packet)
{
	unsigned char kind = packet.data[0];
	if (kind == ID_TIMESTAMP) {
		kind = packet.data[1 + sizeof(RakNetTime)];
	}
	return kind;
}

std::ostream& packetHeader(std::ostream& os, const Packet& packet)
{
	os << "[" << packet.systemAddress.ToString() << "] ";
	return os;
}

std::auto_ptr<BitStream> packetStream(const Packet& packet)
{
	std::auto_ptr<BitStream> bs(new BitStream(packet.data, packet.length, false));
	// Skip header
	unsigned char timestamped;
	bs->Read(timestamped);
	if (timestamped == ID_TIMESTAMP) {
		RakNetTime timestamp;
		bs->Read(timestamp);
		unsigned char unused;
		bs->Read(unused);
	}
	return bs;
}
