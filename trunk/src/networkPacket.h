#ifndef NETWORK_PACKET_H_
#define NETWORK_PACKET_H_

#include <RakNet/BitStream.h>
#include <RakNet/RakNetTypes.h>
#include <RakNet/RakPeerInterface.h>
#include <string>
#include <vector>
#include <boost/foreach.hpp>
#include "vector2D.h"
#include "frameTimer.h"

using RakNet::BitStream;
class User;

// Extra serialization functions
template<typename T>
void serial(BitStream& bs, bool write, T& data)
{
	bs.SerializeCompressed(bs, write, data);
}
template<typename T>
void serial(BitStream& bs, bool write, std::vector<T>& data)
{
	if (write) {
		bs.WriteCompressed(data.size());
		BOOST_FOREACH(T& value, data) {
			serial(bs, write, value);
		}
	}
	else {
		typedef typename std::vector<T>::size_type Size;
		Size size;
		bs.ReadCompressed(size);
		data.resize(size);
		for (Size i = 0; i < size; ++i) {
			serial(bs, write, data[i]);
		}
	}
	bs.SerializeCompressed(write, data);
}
void serial(BitStream& bs, bool write, std::string& data);
void serial(BitStream& bs, bool write, Vector2D& data);

// Parameters for a network packet.
// See similarly-named NetworkPacket functions.
struct NetworkParams {

	NetworkParams(unsigned char kind,
			PacketPriority priority=LOW_PRIORITY,
			PacketReliability reliability=RELIABLE_ORDERED,
			char orderingChannel=0,
			bool useTimestamp=false):
		kind(kind), priority(priority), reliability(reliability),
		orderingChannel(orderingChannel), useTimestamp(useTimestamp)
	{
	}

	unsigned char kind;
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
	unsigned char kind() const { return params().kind; }

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
	virtual void respondClient() const = 0;
#else
	virtual void respondServer() const = 0;
#endif

private:
	RakNetTime _timestamp;
	User *_sender;
};

#endif
