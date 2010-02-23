#include "clientWorldMap.h"
#include "serializationException.h"
#include <boost/foreach.hpp>

ClientWorldMap::~ClientWorldMap()
{
	BOOST_FOREACH(const CellMap::value_type& cell, _cellMap) {
		delete cell.second;
	}
}

ClientWorldMap::Cell& ClientWorldMap::getCell(const IVector2D& v)
{
	std::pair<CellMap::iterator, bool> result = _cellMap.insert(CellMap::value_type(v, 0));
	if (result.second) {
		result.first->second = new Cell(boost::extents[CELL_DIMENSIONS.y][CELL_DIMENSIONS.x]);
	}
	return *result.first->second;
}

void ClientWorldMap::loadCell(const IVector2D& v, const TileDataVector& tileData)
{
	Cell& cell = getCell(v);
	if (tileData.size() != cell.num_elements()) {
		SERIALIZATION_EXCEPTION(
			"Wrong amount of cell tile data given to load into a new cell. " <<
			tileData.size() << " found, " << cell.num_elements() << " needed.");
	}
	
	ClientTile *iter = cell.data();
	BOOST_FOREACH(TileType type, tileData) {
		*iter++ = ClientTile(TileType(type), DIRTY_TILE);
	}
	assert(iter == cell.data() + cell.num_elements());

	// Dirty the edges of the nearby cells.
	Cell& left =   getCell(v - IVector2D(1, 0));
	Cell& right =  getCell(v + IVector2D(1, 0));
	Cell& top =    getCell(v - IVector2D(0, 1));
	Cell& bottom = getCell(v + IVector2D(0, 1));
	for (int32_t i = 0; i < CELL_DIMENSIONS.y; ++i) {
		left[i][CELL_DIMENSIONS.x - 1].sprite = DIRTY_TILE;
		right[i][0].sprite = DIRTY_TILE;
	}
	for (int32_t i = 0; i < CELL_DIMENSIONS.x; ++i) {
		top[CELL_DIMENSIONS.y - 1][i].sprite = DIRTY_TILE;
		bottom[0][i].sprite = DIRTY_TILE;
	}
}

void ClientWorldMap::discardCell(const IVector2D& v)
{
	CellMap::iterator found = _cellMap.find(v);
	delete found->second;
    _cellMap.erase(found);
}

TileType ClientWorldMap::getTile(const IVector2D& v) const
{
	return const_cast<ClientWorldMap&>(*this).getTile(v).type;
}

ClientTile& ClientWorldMap::getTile(const IVector2D& v)
{
	IVector2D cellIndex = v.memberwiseDiv(CELL_DIMENSIONS);
	IVector2D tileIndex = v.memberwiseMod(CELL_DIMENSIONS);

	if (v.x < 0 && tileIndex.x != 0) {
		cellIndex.x -= 1;
		tileIndex.x += CELL_DIMENSIONS.x;
	}
	if (v.y < 0 && tileIndex.y != 0) {
		cellIndex.y -= 1;
		tileIndex.y += CELL_DIMENSIONS.y;
	}
	
	return getCell(cellIndex)[tileIndex.y][tileIndex.x];
}

void ClientWorldMap::setTile(const IVector2D& v, TileType type)
{
	ClientTile& tile = getTile(v);
	if (tile.type == type) {
		tile.type = type;
		tile.sprite = DIRTY_TILE;

		// Mark nearby tiles as dirty as well
		getTile(v - IVector2D(1, 0)).sprite = DIRTY_TILE;
		getTile(v + IVector2D(1, 0)).sprite = DIRTY_TILE;
		getTile(v - IVector2D(0, 1)).sprite = DIRTY_TILE;
		getTile(v + IVector2D(0, 1)).sprite = DIRTY_TILE;
	}
}

TileSprite ClientWorldMap::getSprite(const IVector2D& v)
{
	ClientTile& tile = const_cast<ClientWorldMap&>(*this).getTile(v);
	if (tile.sprite == DIRTY_TILE) {
        TileMatrix matrix;
        matrix[TOP][LEFT]      = getTile(v + IVector2D(-1, -1)).type;
        matrix[TOP][CENTER]    = getTile(v + IVector2D( 0, -1)).type;
        matrix[TOP][RIGHT]     = getTile(v + IVector2D( 1, -1)).type;
        matrix[CENTER][LEFT]   = getTile(v + IVector2D(-1,  0)).type;
        matrix[CENTER][CENTER] = tile.type;
        matrix[CENTER][RIGHT]  = getTile(v + IVector2D( 1,  0)).type;
        matrix[BOTTOM][LEFT]   = getTile(v + IVector2D(-1,  1)).type;
        matrix[BOTTOM][CENTER] = getTile(v + IVector2D( 0,  1)).type;
        matrix[BOTTOM][RIGHT]  = getTile(v + IVector2D( 1,  1)).type;

		tile.sprite = determineTileSprite(matrix);
	}
	assert(tile.sprite != DIRTY_TILE);
    return tile.sprite;
}
