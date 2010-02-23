#ifndef TILE_TYPES_H_
#define TILE_TYPES_H_

#include <boost/multi_array.hpp>
#include <boost/unordered_map.hpp>
#include <RakNet/NativeTypes.h>
#include "vector2D.h"

extern const IVector2D CELL_DIMENSIONS;

typedef uint8_t TileType;

// Maybe change later
// Assume less than 255 types.
enum TileTypeEnum {
	TILE_ETHER,
	TILE_DIRT,
	TILE_GRASS,
	TILE_COUNT // number of tiles; keep last
};
BOOST_STATIC_ASSERT(TILE_COUNT < 256);

typedef boost::uint16_t TileSprite;

const TileSprite DIRTY_TILE = -1;

const int LEFT   = -1;
const int RIGHT  =  1;
const int TOP    = -1;
const int BOTTOM =  1;
const int CENTER =  0;

struct TileMatrix {
    struct TileRow {
        TileType left, center, right;
        TileType& operator[](int id)
        {
            if ( id == LEFT )
                return left;
            if ( id == RIGHT )
                return right;
            if ( id == CENTER )
                return center;

            // Only know about 3x3 grid!
            assert(false);
            return center;
        }
        const TileType& operator[](int id) const
        {
            return const_cast<TileRow*>(this)->operator[](id);
        }
    };

    TileRow top, center, bottom;
    TileRow& operator[](int id)
    {
        if ( id == TOP )
            return top;
        if ( id == BOTTOM )
            return bottom;
        if ( id == CENTER )
            return center;

        // Only know about 3x3 grid!
        assert(false);
        return center;
    }
    const TileRow& operator[](int id) const
    {
        return const_cast<TileMatrix*>(this)->operator[](id);
    }
};

// Given the center tile and surrounding tiles,
// come up with a (possibly random) appropriate sprite.
TileSprite determineTileSprite(const TileMatrix& tiles);

// Convert from tile coordinates to cell coordinates.
IVector2D toCellCoordinates(const IVector2D& tile);
// Convert from cell coordinates to tile coordinates.
IVector2D toTileCoordinates(const IVector2D& cell);

#endif
