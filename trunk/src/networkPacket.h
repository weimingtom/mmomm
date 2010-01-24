#ifndef NETWORK_PACKET_H_
#define NETWORK_PACKET_H_

#include <RakNet/BitStream.h>
#include <RakNet/RakNetTypes.h>
#include <RakNet/RakPeerInterface.h>
#include <boost/scoped_ptr.hpp>

using RakNet::BitStream;

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
	void read(Packet *packet);
	
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
	RakNetTime timestamp() const { assert(useTimestamp()); return _timestamp; }
	
	// The system address of the sender (only available upon receipt).
	const SystemAddress& address() const { return _address; }
	
	// Specify the parameters to use for sending this packet.
	virtual NetworkParams params() const = 0;

	// Serialize or deserialize data from the specific packet type.
	// bs: the stream used to serialize the data
	// write: true means to write to the bitstream, false means to read from it
	virtual void serialize(BitStream& bs, bool write) = 0;

private:
	RakNetTime _timestamp;
	SystemAddress _address;
};

// Wrapper type for simple messages.
template<unsigned char Kind,
	PacketPriority Priority=LOW_PRIORITY,
	PacketReliability Reliability=RELIABLE_ORDERED,
	char OrderingChannel=0,
	bool UseTimestamp=false>
class SimplePacket: NetworkPacket {
public:
	// Simplest possible implementations.
	virtual NetworkParams params() const
		{ return PacketParams(Kind, Priority, Reliability,
			OrderingChannel, UseTimestamp); }
	virtual void serialize(BitStream& bs, bool write) { (void)bs; (void)write; }
};

#endif
