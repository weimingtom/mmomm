#ifndef WORLD_MAP_H_
#define WORLD_MAP_H_

#include <boost/multi_array.hpp>
#include <boost/unordered_map.hpp>
#include <RakNet/NativeTypes.h>
#include "vector2D.h"

// Maybe change later
// Assume less than 255 types.
enum Tile {
	TILE_ETHER,
	TILE_DIRT,
	TILE_GRASS,
	TILE_COUNT // number of tiles; keep last
};
BOOST_STATIC_ASSERT(TILE_COUNT < 256);

class WorldCell {
public:

    const static std::size_t WIDTH  = 32;
    const static std::size_t HEIGHT = 32;

    WorldCell();
    ~WorldCell();

    const Tile& GetTile(std::size_t x, std::size_t y) const;
    void SetTile(std::size_t x, std::size_t y, const Tile& tile);

private:

    typedef boost::multi_array< Tile, 2 > Data;
    Data _data;

};

class WorldMap {
public:

    // If no cell exists at the specified position, a blank cell is created and returned.
    const WorldCell& GetCell(long x, long y);

    // Copy cell data over specified position.
    void SetCell(long x, long y, const WorldCell& cell);

    // Removes the given cell from memory, if it exists.
    void DiscardCell(long x, long y);

    // Directly access the correct tile of the underlying cell at the given position.
    const Tile& GetTile(long x, long y);
    void SetTile(long x, long y, const Tile& tile);

private:

    typedef boost::unordered_map< IVector2D, WorldCell > Map;
    Map _map;

};

#endif
