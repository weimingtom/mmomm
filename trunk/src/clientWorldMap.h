#ifndef CLIENT_WORLD_MAP_H_
#define CLIENT_WORLD_MAP_H_

#include <boost/multi_array.hpp>
#include <boost/unordered_map.hpp>
#include <RakNet/NativeTypes.h>
#include "rect.h"
#include "tileTypes.h"
#include "vector2D.h"

// Data for a tile.
struct ClientTile {
	ClientTile(): sprite(DIRTY_TILE), type(TILE_ETHER) { }
	ClientTile(TileType type, TileSprite sprite)
		: sprite(sprite), type(type) { }
	
	// The index of the sprite to use.
	TileSprite sprite;
	// The type of the tile which determines its behavior.
	TileType type;
};

class ClientWorldMap {
public:
	
	~ClientWorldMap();

	// Loads tile data into a given (possibly new) cell and returns it.
	typedef std::vector<TileType> TileDataVector;
	void loadCell(const IVector2D& v, const TileDataVector& tileData);
	
    // Removes the given cell from memory, if it exists.
    void discardCell(const IVector2D& v);
	
    // Directly access the correct tile of the underlying cell at the given position.
    TileType getTile(const IVector2D& v) const;
	void setTile(const IVector2D& v, TileType type);

    // Determine the sprite at the given position, calculating if necessary.
    TileSprite getSprite(const IVector2D& v);
	
private:
	
    typedef boost::multi_array< ClientTile, 2 > Cell;
    typedef boost::unordered_map< IVector2D, Cell* > CellMap;
	CellMap _cellMap;
	
	Cell& getCell(const IVector2D& v);
    ClientTile& getTile(const IVector2D& v);
};

#endif
