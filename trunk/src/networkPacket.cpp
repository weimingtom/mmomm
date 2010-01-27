#include "networkPacket.h"
#include <RakNet/MessageIdentifiers.h>
#include <RakNet/GetTime.h>
#include <RakNet/StringCompressor.h>
#include <string>

// Initializes the string compressor.
struct StringCompressorInitializer {
	StringCompressorInitializer()
	{
		StringCompressor::AddReference();
		// NOTE: Could add custom compression table.
	}
};

StringCompressorInitializer stringCompressorInitializer;

void serial(BitStream& bs, bool write, std::string& data)
{
	const int bufferSize = 256;
	if (write) {
		StringCompressor::Instance()->EncodeString(data.c_str(), bufferSize, &bs);
	}
	else {
		char buffer[bufferSize+1];
		StringCompressor::Instance()->DecodeString(buffer, bufferSize, &bs);
		data = buffer;
	}
}


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
		unsigned char timestamped;
        // Hey nagel I am getting a crash at this line just below; happens when you have connected
        // to localhost but you're not actually running a server (connection to localhost appears 
        // to work okay, but it crashes after a little while
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

