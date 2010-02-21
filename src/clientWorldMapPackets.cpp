#include "worldMapPackets.h"
#include "clientWorldInstance.h"
#include "clientWorldMap.h"
#include <iostream>

void CellPacket::respondClient() const
{
	ClientWorldMap& map = ClientWorldInstance::current().getWorldMap();
	const IVector2D tilePosition = referencePoint();
	const IVector2D cellPosition = toCellCoordinates(tilePosition);
	
	BOOST_FOREACH(const CellUpdate& update, cell()) {
		map.loadCell(update.position + cellPosition, update.tiles);
	}
	
	BOOST_FOREACH(const TileUpdate& update, tile()) {
		map.setTile(update.position + tilePosition, update.tile);
	}
}

