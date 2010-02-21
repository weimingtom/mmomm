#include "tileTypes.h"

const IVector2D CELL_DIMENSIONS = IVector2D(32, 32);

TileSprite determineTileSprite(
	TileType center, 
	TileType left,
	TileType top, 
	TileType right, 
	TileType bottom)
{
	// TODO: Implement sprite tile based on context.
	return 0;
}

void renderTileSprite(TileSprite sprite, const Vector2D& position)
{
//    renderer.drawClippedImage(anim->getImage().get(), renderPosition, clip);
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
