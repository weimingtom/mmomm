#ifndef WORLDMAP_H_
#define WORLDMAP_H_

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

    const WorldCell& GetCell(long x, long y);
    void SetCell(long x, long y, const WorldCell& cell);

    const Tile& GetTile(long x, long y);
    void SetTile(long x, long y, const Tile& tile);

private:

    typedef boost::unordered_map< CellCoord, WorldCell, CellCoord::Hash > Map;
    Map _map;

};

#endif