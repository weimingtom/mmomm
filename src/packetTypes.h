#ifndef PACKET_TYPES_H_
#define PACKET_TYPES_H_

#include <RakNet/MessageIdentifiers.h>

enum PacketType {
	
	ID_ACCOUNT_LOGIN = ID_USER_PACKET_ENUM,
	ID_ACCOUNT_SUCCESS,
	ID_ACCOUNT_FAILURE,

	// From this point on, the messages aren't used internally.
	ID_NOT_INTERNAL,
};

#endif