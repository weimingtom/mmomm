#ifndef COLLISION_PACKETS_H_
#define COLLISION_PACKETS_H_

// Packets linked with actors and collision.

#include <string>
#include <iostream>
#include <boost/static_assert.hpp>
#include "collision.h"
#include "networkPacket.h"
#include "packetTypes.h"
#include "serial.h"
#include "vector2D.h"

// Gives information about the initial state of an object.
struct CreationUpdate {
	ActorID id;
	Rect offsetRect;
	Vector2D velocity;
	uint8_t sprite;
    bool isClientPlayer;
    std::string name;
};

// Informs the player that an entity is no longer relevant
struct DestructionUpdate {
	ActorID id;
};

// Informs the player of an entity's new position and velocity
struct MovementUpdate {
	ActorID id;
	Vector2D displacement;
	Vector2D velocity;
};

inline void serial(BitStream& bs, bool write, CreationUpdate& data)
{
	serialFull(bs, write, data.id);
	serialDisplacement(bs, write, data.offsetRect);
	serialVelocity(bs, write, data.velocity);
	serial(bs, write, data.sprite);
    serial(bs, write, data.isClientPlayer);
    serial(bs, write, data.name);
}

inline void serial(BitStream& bs, bool write, DestructionUpdate& data)
{
	serialFull(bs, write, data.id);
}

inline void serial(BitStream& bs, bool write, MovementUpdate& data)
{
	serialFull(bs, write, data.id);
	serialDisplacement(bs, write, data.displacement);
	serialVelocity(bs, write, data.velocity);
}

// Occurs to inform about new entities and destroyed entities.
class CreationPacket: public NetworkPacket {
public:

	CreationPacket() { }
	// Initialize from a series of movementUpdates
	template<typename CreationIterator, typename DestructionIterator>
	CreationPacket(const Vector2D& referencePoint,
			CreationIterator createBegin, CreationIterator createEnd,
			DestructionIterator destructBegin, DestructionIterator destructEnd)
		: _referencePoint(referencePoint)
		, _creation(createBegin, createEnd)
		, _destruction(destructBegin, destructEnd) { }

	NetworkParams params() const
	{
		return NetworkParams(ID_COLLISION_CREATE, HIGH_PRIORITY,
				RELIABLE, ORDER_DEFAULT, true);
	}

	// Respond to object creation on client.
	void respondClient() const;

	// Invalid on server.

	// Serialization function.
	void serialize(BitStream& bs, bool write)
	{
		serialPosition(bs, write, _referencePoint);
		serial(bs, write, _creation);
		serial(bs, write, _destruction);
	}
	
	const Vector2D& referencePoint() const { return _referencePoint; }

	typedef std::vector<CreationUpdate> CreationList;
	const CreationList& creation() const { return _creation; }

	typedef std::vector<DestructionUpdate> DestructionList;
	const DestructionList& destruction() const { return _destruction; }

private:
	Vector2D _referencePoint;
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
	MovementPacket(const Vector2D& referencePoint,
			MovementIterator moveBegin, MovementIterator moveEnd)
		: _referencePoint(referencePoint)
		, _movement(moveBegin, moveEnd) { }

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
		serialPosition(bs, write, _referencePoint);
		serial(bs, write, _movement);
	}

	const Vector2D& referencePoint() const { return _referencePoint; }

	typedef std::vector<MovementUpdate> MovementList;
	const MovementList& movement() const { return _movement; }

private:
	Vector2D _referencePoint;
	MovementList _movement;
};


#endif
