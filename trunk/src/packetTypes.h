#ifndef PACKET_TYPES_H_
#define PACKET_TYPES_H_

#include <RakNet/MessageIdentifiers.h>

enum PacketType {
	
	// Internal login/account messages
	ID_ACCOUNT_LOGIN = ID_USER_PACKET_ENUM,
	ID_ACCOUNT_SUCCESS,
	ID_ACCOUNT_FAILURE,

	// From this point on, the messages aren't used internally.
	ID_NOT_INTERNAL,
	
	// Chat messages
	ID_CHAT_MESSAGE,
	
	// World updates
	ID_COLLISION_CREATE,
	ID_COLLISION_UPDATE
};

// Ordering channels; max of 32
enum OrderedChannel {
	// Default channel
	ORDER_DEFAULT = 0,
	// Chat message band
	ORDER_CHAT = 1,
};

// Sequence channels; max of 32
enum SequencedChannel {
	// Default band
	SEQUENCE_DEFAULT = 0,
	// World-specific updates
	SEQUENCE_WORLD = 1,
};

#endif