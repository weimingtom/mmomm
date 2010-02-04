#ifndef CHAT_H_
#define CHAT_H_

// Packets linked with chatting.

#include "networkPacket.h"
#include "packetTypes.h"
#include <string>

// Occurs when a connection has been established.
class ChatMessagePacket: public NetworkPacket {
public:
	enum TYPE
    {
        CHAT_MESSAGE_GENERAL = 0,
        CHAT_MESSAGE_PRIVATE,
        CHAT_MESSAGE_SERVER
    };

	ChatMessagePacket() { }
	ChatMessagePacket(const std::string& from, const std::string& target,
		const std::string& message, const TYPE type)
		: _from(from), _target(target), _message(message), _type(type) { }

	NetworkParams params() const
	{
		return NetworkParams(ID_CHAT_MESSAGE, LOW_PRIORITY, RELIABLE_ORDERED, ORDER_CHAT);
	}

	// Respond to successful connection on client.
	void respondClient() const;

	// Respond to a new incoming connection.
	void respondServer() const;

	// Serialization function.
	void serialize(BitStream& bs, bool write)
	{
		serial(bs, write, _from);
		serial(bs, write, _target);
		serial(bs, write, _message);
		serial(bs, write, _type);
	}

	// The person who sent the message (from server only)
	std::string from() const { return _from; }
	// The person or place the message is sent to.
	std::string target() const { return _target; }
	// The message that was sent.
	std::string message() const { return _message; }
	// The type of message sent.
	TYPE type() const { return _type; }

private:
	std::string _from;
	std::string _target;
	std::string _message;
	TYPE _type;
};


#endif
