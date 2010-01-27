#include "initializePacketManager.h"
#include "networkPacketManager.h"

#include "chat.h"
#include "collisionPackets.h"

#define MMOMM_DECLARE_KIND(type) \
	manager.assign(getKind<type>(), \
		NetworkPacketManager::creation<type>)

template<typename PacketKind>
unsigned char getKind()
{
	PacketKind packet;
	return packet.kind();
}

void initializePacketManager(NetworkPacketManager& manager)
{
	MMOMM_DECLARE_KIND(ChatMessagePacket);
	MMOMM_DECLARE_KIND(CreationPacket);
	MMOMM_DECLARE_KIND(MovementPacket);
}
