#ifndef WORLD_MAP_PACKETS_H_
#define WORLD_MAP_PACKETS_H_

// Packets involving world map tiles.

#include <string>
#include <iostream>
#include <boost/static_assert.hpp>
#include "tileTypes.h"
#include "networkPacket.h"
#include "packetTypes.h"
#include "serial.h"
#include "vector2D.h"

// Stores data for an entire cell
struct CellUpdate {
	// Position of cell, in cell coordinates.
	IVector2D position;
	std::vector<TileType> tiles;
};

// Stores data for a single tile.
struct TileUpdate {
	// Position of cell, in world coordinates.
	IVector2D position;
	TileType tile;
};

inline void serial(BitStream& bs, bool write, CellUpdate& data)
{
	serialDisplacement(bs, write, data.position);
	serialFixed(bs, write, data.tiles, CELL_DIMENSIONS.y * CELL_DIMENSIONS.x);
}

inline void serial(BitStream& bs, bool write, TileUpdate& data)
{
	serialDisplacement(bs, write, data.position);
	serialFull(bs, write, data.tile);
}

// Occurs to inform about new and updated cells.
class CellPacket: public NetworkPacket {
public:

	CellPacket() { }
	// Initialize from a series of CellUpdate
	template<typename CellIterator, typename TileIterator>
	CellPacket(const IVector2D& referencePoint,
			CellIterator cellBegin, CellIterator cellEnd,
			TileIterator tileBegin, TileIterator tileEnd)
		: _referencePoint(referencePoint)
		, _cell(cellBegin, cellEnd)
		, _tile(tileBegin, tileEnd) { }

	NetworkParams params() const
	{
		return NetworkParams(ID_WORLD_MAP_CELL, MEDIUM_PRIORITY,
				RELIABLE_ORDERED, ORDER_CELL, false);
	}

	// Respond to object creation on client.
	void respondClient() const;

	// Invalid message on server.
	
	// Serialization function.
	void serialize(BitStream& bs, bool write)
	{
		serialPosition(bs, write, _referencePoint);
		serial(bs, write, _cell);
		serial(bs, write, _tile);
	}
	
	const IVector2D& referencePoint() const { return _referencePoint; }
	
	typedef std::vector<CellUpdate> CellList;
	const CellList& cell() const { return _cell; }
	
	typedef std::vector<TileUpdate> TileList;
	const TileList& tile() const { return _tile; }
	
private:
	IVector2D _referencePoint;
	CellList _cell;
	TileList _tile;
};

#endif
