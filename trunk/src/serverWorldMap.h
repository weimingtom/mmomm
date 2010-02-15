#ifndef SERVER_WORLD_MAP_H_
#define SERVER_WORLD_MAP_H_

#include <boost/multi_array.hpp>
#include <boost/unordered_map.hpp>
#include <RakNet/NativeTypes.h>
#include "tileTypes.h"
#include "vector2D.h"

class ServerWorldMap {
public:
	
	~ServerWorldMap();

	// Saves/loads tile data from/to a given (possibly new) cell and returns it.
	typedef std::vector<uint8_t> TileDataVector;
	void loadCell(const IVector2D& v, const TileDataVector& tileData);
	void saveCell(const IVector2D& v, TileDataVector& buffer);
	
    // Removes the given cell from memory, if it exists.
    void discardCell(const IVector2D& v);
	
    // Directly access the correct tile of the underlying cell at the given position.
    TileType getTile(const IVector2D& v) const;
	void setTile(const IVector2D& v, TileType type);
	
private:

    typedef boost::multi_array< uint8_t, 2 > Cell;
    typedef boost::unordered_map< IVector2D, Cell* > CellMap;
	CellMap _cellMap;

    uint8_t& getTile(const IVector2D& v);
    Cell& getCell(const IVector2D& v);
};

#endif
