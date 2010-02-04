#ifndef COLLISION_PACKETS_H_
#define COLLISION_PACKETS_H_

// Packets linked with chatting.

#include "networkPacket.h"
#include "packetTypes.h"
#include "vector2D.h"
#include "collision.h"
#include <string>
#include <iostream>

// Gives information about the initial state of an object.
struct CreationUpdate {
	ActorID id;
	Rect rect;
	Vector2D velocity;
	int sprite;
    bool isClientPlayer;
    std::string name;
};

inline void serial(BitStream& bs, bool write, CreationUpdate& data)
{
	bs.Serialize(write, data.id);
	serial(bs, write, data.rect);
	serial(bs, write, data.velocity);
	serial(bs, write, data.sprite);
    serial(bs, write, data.isClientPlayer);
    serial(bs, write, data.name);
}

// Informs the player that an entity is no longer relevant
struct DestructionUpdate {
	ActorID id;
};

inline void serial(BitStream& bs, bool write, DestructionUpdate& data)
{
	bs.Serialize(write, data.id);
}

// Informs the player of an entity's new position and velocity
struct MovementUpdate {
	ActorID id;
	Vector2D position;
	Vector2D velocity;
};

inline void serial(BitStream& bs, bool write, MovementUpdate& data)
{
	bs.Serialize(write, data.id);
	serial(bs, write, data.position);
	serial(bs, write, data.velocity);
}

// Occurs to inform about new entities and destroyed entities.
class CreationPacket: public NetworkPacket {
public:

	CreationPacket() { }
	// Initialize from a series of movementUpdates
	template<typename CreationIterator, typename DestructionIterator>
	CreationPacket(CreationIterator createBegin, CreationIterator createEnd,
			DestructionIterator destructBegin, DestructionIterator destructEnd)
		: _creation(createBegin, createEnd)
		, _destruction(destructBegin, destructEnd) { }

	NetworkParams params() const
	{
		return NetworkParams(ID_COLLISION_CREATE, HIGH_PRIORITY,
				RELIABLE, ORDER_DEFAULT, true);
	}

	// Respond to object creation on client.
	void respondClient() const;

	// Invalid message on server.
	void respondServer() const { }

	// Serialization function.
	void serialize(BitStream& bs, bool write)
	{
		serial(bs, write, _creation);
		serial(bs, write, _destruction);
	}

	typedef std::vector<CreationUpdate> CreationList;
	const CreationList& creation() const { return _creation; }

	typedef std::vector<DestructionUpdate> DestructionList;
	const DestructionList& destruction() const { return _destruction; }

private:
	CreationList _creation;
	DestructionList _destruction;
};

// Occurs to inform about entities moving around.
// May be processed before a CreationUpdate or after a DestructionUpdate
class MovementPacket: public NetworkPacket {
public:

	MovementPacket() { }
	// Initialize from a series of movementUpdates
	template<typename MovementIterator>
	MovementPacket(MovementIterator moveBegin, MovementIterator moveEnd)
		: _movement(moveBegin, moveEnd) { }

	NetworkParams params() const
	{
		// Could be just UNRELIABLE, though packet inversion would look odd
		return NetworkParams(ID_COLLISION_UPDATE, HIGH_PRIORITY,
				UNRELIABLE, SEQUENCE_DEFAULT, true);
	}

	// Respond to actor movement on client.
	void respondClient() const;

	// Respond to player movement on server.
	void respondServer() const;

	// Serialization function.
	void serialize(BitStream& bs, bool write)
	{
		serial(bs, write, _movement);
	}

	typedef std::vector<MovementUpdate> MovementList;
	const MovementList& movement() const { return _movement; }

private:
	MovementList _movement;
};


#endif
