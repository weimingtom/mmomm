#include "tileTypes.h"

const IVector2D CELL_DIMENSIONS = IVector2D(32, 32);

TileSprite determineTileSprite(const TileMatrix& tiles, TileType targetType, TileSprite baseIndex)
{
    bool tl = tiles[TOP][LEFT]      == targetType;
    bool t  = tiles[TOP][CENTER]    == targetType;
    bool tr = tiles[TOP][RIGHT]     == targetType;
    bool l  = tiles[CENTER][LEFT]   == targetType;
    bool r  = tiles[CENTER][RIGHT]  == targetType;
    bool bl = tiles[BOTTOM][LEFT]   == targetType;
    bool b  = tiles[BOTTOM][CENTER] == targetType;
    bool br = tiles[BOTTOM][RIGHT]  == targetType;

    if ( !tl && !tr && !bl && !br && l && r && t && b )
        return baseIndex + 0;
    if ( tl && tr && bl && br && l && r && t && b )
        return baseIndex + 1;

    if ( !l && !r && !t && !b )
        return baseIndex + 2;
    if ( !l && r && !t && !b )
        return baseIndex + 3;
    if ( l && !r && !t && !b )
        return baseIndex + 4;
    if ( !l && !r && !t && b )
        return baseIndex + 5;
    if ( !l && !r && t && !b )
        return baseIndex + 6;

    if ( !l && r && !t && b && br )
        return baseIndex + 7;
    if ( !l && r && !t && b && !br )
        return baseIndex + 9;
    if ( l && !r && !t && b && bl )
        return baseIndex + 8;
    if ( l && !r && !t && b && !bl )
        return baseIndex + 10;
    if ( !l && r && t && !b && tr )
        return baseIndex + 15;
    if ( !l && r && t && !b && !tr )
        return baseIndex + 17;
    if ( l && !r && t && !b && tl )
        return baseIndex + 16;
    if ( l && !r && t && !b && !tl )
        return baseIndex + 18;

    if ( !l && r && t && b && tr && br )
        return baseIndex + 11;
    if ( l && !r && t && b && tl && bl )
        return baseIndex + 12;
    if ( l && r && !t && b && bl && br )
        return baseIndex + 13;
    if ( l && r && t && !b && tl && tr )
        return baseIndex + 14;

    if ( !l && r && t && b && !tr && !br )
        return baseIndex + 19;
    if ( l && !r && t && b && !tl && !bl )
        return baseIndex + 20;
    if ( l && r && !t && b && !bl && !br )
        return baseIndex + 21;
    if ( l && r && t && !b && !tl && !tr )
        return baseIndex + 22;

    if ( !l && r && t && b && tr && !br )
        return baseIndex + 27;
    if ( l && !r && t && b && tl && !bl )
        return baseIndex + 28;
    if ( l && r && !t && b && !bl && br )
        return baseIndex + 29;
    if ( l && r && t && !b && !tl && tr )
        return baseIndex + 30;

    if ( !l && r && t && b && !tr && br )
        return baseIndex + 35;
    if ( l && !r && t && b && !tl && bl )
        return baseIndex + 36;
    if ( l && r && !t && b && bl && !br )
        return baseIndex + 37;
    if ( l && r && t && !b && tl && !tr )
        return baseIndex + 38;

    if ( tl && !tr && !bl && !br && l && r && t && b )
        return baseIndex + 23;
    if ( !tl && tr && !bl && !br && l && r && t && b )
        return baseIndex + 24;
    if ( !tl && !tr && !bl && br && l && r && t && b )
        return baseIndex + 25;
    if ( !tl && !tr && bl && !br && l && r && t && b )
        return baseIndex + 26;

    if ( tl && tr && !bl && !br && l && r && t && b )
        return baseIndex + 29;
    if ( !tl && !tr && bl && br && l && r && t && b )
        return baseIndex + 30;
    if ( tl && !tr && bl && !br && l && r && t && b )
        return baseIndex + 31;
    if ( !tl && tr && !bl && br && l && r && t && b )
        return baseIndex + 32;

    if ( !tl && tr && bl && br && l && r && t && b )
        return baseIndex + 39;
    if ( tl && !tr && bl && br && l && r && t && b )
        return baseIndex + 40;
    if ( tl && tr && bl && !br && l && r && t && b )
        return baseIndex + 41;
    if ( tl && tr && !bl && br && l && r && t && b )
        return baseIndex + 42;

    if ( l && r && !t && !b )
        return baseIndex + 43;
    if ( !l && !r && t && b )
        return baseIndex + 44;

    // Something is wrong, tell seiken (I will cry)
    assert(false);
    return 0;
}

TileSprite determineTileSprite(const TileMatrix& tiles)
{
    if ( tiles[CENTER][CENTER] == TILE_ETHER )
        return 0;
    if ( tiles[CENTER][CENTER] == TILE_GRASS )
        return 46;
    if ( tiles[CENTER][CENTER] == TILE_DIRT )
        return determineTileSprite(tiles, TILE_DIRT, 1);

    // Invalid tile type!
	assert(false);
    return 0;
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
