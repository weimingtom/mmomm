#include "worldMap.h"

WorldCell::WorldCell()
: _data(boost::extents[HEIGHT][WIDTH])
{
}

WorldCell::~WorldCell()
{
}

const Tile& WorldCell::GetTile(std::size_t x, std::size_t y) const
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < WIDTH);
    assert(y < HEIGHT);
    return _data[y][x];
}

void WorldCell::SetTile(std::size_t x, std::size_t y, const Tile& tile)
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < WIDTH);
    assert(y < HEIGHT);
    _data[y][x] = tile;
}

const WorldCell& WorldMap::GetCell(long x, long y)
{
    return _map[CellCoord(x, y)];
}

void WorldMap::SetCell(long x, long y, const WorldCell& cell)
{
    _map[CellCoord(x, y)] = cell;
}

void WorldMap::DiscardCell(long x, long y)
{
    _map.erase(CellCoord(x, y));
}

const Tile& WorldMap::GetTile(long x, long y)
{
    long uWidth  = unsigned(WorldCell::WIDTH );
    long uHeight = unsigned(WorldCell::HEIGHT);
    long xCell = x >= 0 ? x / uWidth  : -(-x / uWidth ) - 1;
    long yCell = y >= 0 ? y / uHeight : -(-y / uHeight) - 1;
    long xTile = x >= 0 ? x % uWidth  : uWidth - ((-x - 1) % uWidth ) - 1;
    long yTile = y >= 0 ? y % uHeight : uWidth - ((-y - 1) % uHeight) - 1;
    return _map[CellCoord(xCell, yCell)].GetTile(xTile, yTile);
}

void WorldMap::SetTile(long x, long y, const Tile& tile)
{
    long uWidth  = unsigned(WorldCell::WIDTH );
    long uHeight = unsigned(WorldCell::HEIGHT);
    long xCell = x >= 0 ? x / uWidth  : -(-x / uWidth ) - 1;
    long yCell = y >= 0 ? y / uHeight : -(-y / uHeight) - 1;
    long xTile = x >= 0 ? x % uWidth  : uWidth - ((-x - 1) % uWidth ) - 1;
    long yTile = y >= 0 ? y % uHeight : uWidth - ((-y - 1) % uHeight) - 1;
    _map[CellCoord(xCell, yCell)].SetTile(xTile, yTile, tile);
}