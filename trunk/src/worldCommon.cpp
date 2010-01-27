#include "worldCommon.h"

CellCoord::CellCoord(long x, long y)
{
    this->x = x;
    this->y = y;
}

bool CellCoord::operator==(const CellCoord& a) const
{
    return x == a.x && y == a.y;
}

std::size_t CellCoord::Hash::operator()(const CellCoord& a) const
{
    std::size_t seed = 0;
    boost::hash_combine( seed, a.x );
    boost::hash_combine( seed, a.y );
    return seed;
}