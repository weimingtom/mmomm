#ifndef NETWORK_COMMON_H_
#define NETWORK_COMMON_H_

// Common functions between client and server.

#include "networkPacket.h"
#include <ostream>
#include <memory>

void sendSimplePacket(RakPeerInterface& peer, const NetworkParams& params,
					  const SystemAddress& address);

// Send an encoded bitstream out.
void sendBitStream(RakPeerInterface& peer, const BitStream& bs,
				   const NetworkParams& params, const SystemAddress& address);

unsigned char packetKind(const Packet& packet);

std::ostream& packetHeader(std::ostream& os, const Packet& packet);

std::auto_ptr<BitStream> packetStream(const Packet& packet);

class AutoDepacketer {
public:
	AutoDepacketer(RakPeerInterface& peer, Packet* packet):
		_peer(peer), _packet(packet) { }
	~AutoDepacketer() { if (_packet) _peer.DeallocatePacket(_packet); }

	void reset() { _packet = 0; }

private:
    RakPeerInterface& _peer;
	Packet *_packet;
};

#endif
