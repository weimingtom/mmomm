#ifndef TILE_TYPES_H_
#define TILE_TYPES_H_

#include <boost/multi_array.hpp>
#include <boost/unordered_map.hpp>
#include <RakNet/NativeTypes.h>
#include "vector2D.h"

extern const IVector2D CELL_DIMENSIONS;

// Maybe change later
// Assume less than 255 types.
enum TileType {
	TILE_ETHER,
	TILE_DIRT,
	TILE_GRASS,
	TILE_COUNT // number of tiles; keep last
};
BOOST_STATIC_ASSERT(TILE_COUNT < 256);

typedef uint16_t TileSprite;

const TileSprite DIRTY_TILE = uint16_t(-1);

// Given the center tile and surrounding tiles,
// come up with a (possibly random) appropriate sprite.
TileSprite determineTileSprite(
	TileType center, 
	TileType left, 
	TileType top, 
	TileType right, 
	TileType bottom);

// Render the specified sprite to the screen.
void renderTileSprite(TileSprite sprite, const Vector2D& position);


#endif
