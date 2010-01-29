#ifndef WORLD_MAP_H_
#define WORLD_MAP_H_

#include "worldCommon.h"
#ifdef NDEBUG
#define BOOST_DISABLE_ASSERTS
#endif
#include <boost/multi_array.hpp>
#include <boost/unordered_map.hpp>

// Maybe change later
typedef unsigned int Tile;

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

    typedef boost::unordered_map< CellCoord, WorldCell, CellCoord::Hash > Map;
    Map _map;

};

#endif
