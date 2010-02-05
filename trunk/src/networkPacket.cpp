#include "networkPacket.h"
#include "serial.h"
#include <RakNet/MessageIdentifiers.h>
#include <string>

NetworkPacket::NetworkPacket()
:	_timestamp()
,	_sender()
{
}

void NetworkPacket::read(const Packet *packet, User *user)
{
	_timestamp = 0;
	_sender = user;
	
	if (packet) {
		BitStream stream(packet->data, packet->length, false);
		uint8_t timestamped;
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
		bs.Write(static_cast<uint8_t>(ID_TIMESTAMP));
		bs.Write(FrameTimer::toTimestamp(FrameTimer::current().frameTime()));
	}
	bs.Write(kind());
	
	const_cast<NetworkPacket *>(this)->serialize(bs, true);
}

