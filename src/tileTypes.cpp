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
