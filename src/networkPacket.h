#ifndef NETWORK_PACKET_H_
#define NETWORK_PACKET_H_

#include "frameTimer.h"
#include "serializationException.h"
#include "vector2D.h"
#include <RakNet/BitStream.h>
#include <RakNet/NativeTypes.h>
#include <RakNet/RakNetTypes.h>
#include <RakNet/RakPeerInterface.h>
#include <boost/foreach.hpp>
#include <string>
#include <vector>
#include <typeinfo>

using RakNet::BitStream;
class User;

// Parameters for a network packet.
// See similarly-named NetworkPacket functions.
struct NetworkParams {

	NetworkParams(uint8_t kind,
			PacketPriority priority=LOW_PRIORITY,
			PacketReliability reliability=RELIABLE_ORDERED,
			char orderingChannel=0,
			bool useTimestamp=false):
		kind(kind), priority(priority), reliability(reliability),
		orderingChannel(orderingChannel), useTimestamp(useTimestamp)
	{
	}

	uint8_t kind;
	PacketPriority priority;
	PacketReliability reliability;
	char orderingChannel;
	bool useTimestamp;
};

// Base class for a packet of network data from the other side.
class NetworkPacket {
public:

	// Does basic initialization.
	NetworkPacket();
	virtual ~NetworkPacket() { }

	// Copies the given raw packet into this structure.
	void read(const Packet *packet, User *user = 0);

	// Copies this structure into the given (empty) bitstream.
	void write(BitStream& bs) const;

	// The kind of packet this is.
	uint8_t kind() const { return params().kind; }

	// What priority to send this packet at.
	// Defaults to LOW_PRIORITY.
	PacketPriority priority() const { return params().priority; }

	// What reliability to send this packet with.
	// Defaults to RELIABLE_ORDERED.
	PacketReliability reliability() const { return params().reliability; }

	// Which ordering channel to ship this packet on.
	// 0 to 31; separate sets of channels for ORDERED and SEQUENCED
	// Defaults to 0.
	char orderingChannel() const { return params().orderingChannel; }

	// Whether or not to include a timestamp.
	// Defaults to false.
	bool useTimestamp() const { return params().useTimestamp; }

	// The timestamp on this packet (only available upon receipt).
	// Should only be available if useTimestamp.
	double timestamp() const { assert(useTimestamp()); return FrameTimer::fromTimestamp(_timestamp); }

	// The system address of the sender (only available upon receipt).
	// Server only!
	User& sender() const { assert(_sender); return *_sender; }

	// Specify the parameters to use for sending this packet.
	virtual NetworkParams params() const = 0;

	// Serialize or deserialize data from the specific packet type.
	// bs: the stream used to serialize the data
	// write: true means to write to the bitstream, false means to read from it
	virtual void serialize(BitStream& bs, bool write) { (void)bs; (void)write; }

	// Respond to this packet being received.
	// Implementation is different between client and server.
	// HACK: Use macro to avoid errors for not defining one or the other.
#ifdef MMOMM_CLIENT
	virtual void respondClient() const
	{
		INVALID_PACKET_EXCEPTION(
			"Received unexpected packet on client of type " <<
			typeid(this).name() << ".");
	}
#else
	virtual void respondServer() const
	{
		INVALID_PACKET_EXCEPTION(
			"Received unexpected packet on server of type " <<
			typeid(this).name() << ".");
	}
#endif

private:
	RakNetTime _timestamp;
	User *_sender;
};

#endif
