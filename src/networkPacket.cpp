#include "networkPacket.h"
#include <RakNet/MessageIdentifiers.h>
#include <RakNet/GetTime.h>

NetworkPacket::NetworkPacket()
:	_timestamp()
,	_address(UNASSIGNED_SYSTEM_ADDRESS)
{
}

void NetworkPacket::read(Packet* packet)
{
	_timestamp = 0;
	_address = UNASSIGNED_SYSTEM_ADDRESS;
	
	if (packet) {
		_address = packet->systemAddress;

		BitStream stream(packet->data, packet->length, false);
		unsigned char timestamped;
		stream.Read(timestamped);
		if (timestamped == ID_TIMESTAMP) {
			assert(useTimestamp());
			stream.Read(_timestamp);
			unsigned char unused;
			stream.Read(unused);
		}
		else {
			assert(!useTimestamp());
		}
		
		serialize(stream, false);
	}
}

void NetworkPacket::write(BitStream& bs) const
{
	if (useTimestamp()) {
		bs.Write(static_cast<unsigned char>(ID_TIMESTAMP));
		bs.Write(RakNet::GetTime());
	}
	bs.Write(kind());
	
	const_cast<NetworkPacket *>(this)->serialize(bs, true);
}

