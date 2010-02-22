#include "tileTypes.h"

const IVector2D CELL_DIMENSIONS = IVector2D(32, 32);

TileSprite determineTileSprite(
	TileType center, 
	TileType left,
	TileType top, 
	TileType right, 
	TileType bottom)
{
    if ( center == TILE_ETHER )
        return 0;
    if ( center == TILE_DIRT )
        return 1;
    if ( center == TILE_GRASS )
        return 2;
    // Invalid tile type!
	assert(false);
    return 6; // Favourite number
}

IVector2D toCellCoordinates(const IVector2D& tile)
{
	IVector2D cellIndex = tile.memberwiseMod(CELL_DIMENSIONS);

	if (tile.x < 0) {
		cellIndex.x -= 1;
	}
	if (tile.y < 0) {
		cellIndex.y -= 1;
	}
	return cellIndex;
}

IVector2D toTileCoordinates(const IVector2D& cell)
{
	return cell.memberwiseMult(cell);
}
