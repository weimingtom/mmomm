#include "serverWorldMap.h"
#include "frameTimer.h"
#include "serializationException.h"
#include <boost/foreach.hpp>
#include <functional>

ServerWorldMap::~ServerWorldMap()
{
	BOOST_FOREACH(const CellMap::value_type& cell, _cellMap) {
		delete cell.second;
	}
}

ServerWorldMap::Cell& ServerWorldMap::getCell(const IVector2D& v)
{
	std::pair<CellMap::iterator, bool> result = _cellMap.insert(CellMap::value_type(v, 0));
	if (result.second) {
		result.first->second = new Cell(boost::extents[CELL_DIMENSIONS.y][CELL_DIMENSIONS.x]);
	}
	return *result.first->second;
}

void ServerWorldMap::loadCell(const IVector2D& v, const TileDataVector& tileData)
{
	Cell& cell = getCell(v);
	if (tileData.size() != cell.num_elements()) {
		SERIALIZATION_EXCEPTION(
			"Wrong amount of cell tile data given to load into a new cell. " <<
			tileData.size() << " found, " << cell.num_elements() << " needed.");
	}
	
	std::copy(tileData.begin(), tileData.end(), cell.data());
}

void ServerWorldMap::saveCell(const IVector2D& v, TileDataVector& buffer)
{
	Cell& cell = getCell(v);
	buffer.assign(cell.data(), cell.data() + cell.num_elements());
}

void ServerWorldMap::discardCell(const IVector2D& v)
{
	CellMap::iterator found = _cellMap.find(v);
	delete found->second;
    _cellMap.erase(found);
}

TileType ServerWorldMap::getTile(const IVector2D& v) const
{
	return const_cast<ServerWorldMap&>(*this).getTile(v);
}

TileType& ServerWorldMap::getTile(const IVector2D& v)
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

void ServerWorldMap::setTile(const IVector2D& v, TileType type)
{
	getTile(v) = type;
	_updateMap[toCellCoordinates(v)] = FrameTimer::current().now();
}

double ServerWorldMap::getUpdateTime(const IVector2D& cell) const
{
	return _updateMap[cell];
}
