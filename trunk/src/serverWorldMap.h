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
	typedef std::vector<TileType> TileDataVector;
	void loadCell(const IVector2D& v, const TileDataVector& tileData);
	void saveCell(const IVector2D& v, TileDataVector& buffer);
	
	// Removes the given cell from memory, if it exists.
    void discardCell(const IVector2D& v);
	
    // Directly access the correct tile of the underlying cell at the given position.
    TileType getTile(const IVector2D& v) const;
	void setTile(const IVector2D& v, TileType type);
	
	// Figure out the time that the cell was last updated.
	double getUpdateTime(const IVector2D& cell) const;
	
private:

    typedef boost::multi_array< TileType, 2 > Cell;
    typedef boost::unordered_map< IVector2D, Cell* > CellMap;
	CellMap _cellMap;

	typedef boost::unordered_map< IVector2D, double > UpdateMap;
	mutable UpdateMap _updateMap;

    TileType& getTile(const IVector2D& v);
    Cell& getCell(const IVector2D& v);
};

#endif
