#include "user.h"
#include "actor.h"
#include "collisionPackets.h"
#include "frameTimer.h"
#include "networkServer.h"
#include "serverWorldInstance.h"
#include "serverWorldMap.h"
#include "worldMapPackets.h"
#include <boost/foreach.hpp>
#include <boost/unordered_set.hpp>
#include <algorithm>
#include <cmath>


double User::score(const PacketValue& value, const Vector2D& reference)
{
	const Actor *actor = value.first;
	const Vector2D actorPosition = actor->getPosition();
	
	// Score based on x and y distance
	const Vector2D ASPECT(20.0 / 25.0, 15.0 / 25.0);
	const double DISTANCE_FACTOR = 20;
	
	Vector2D distance = ASPECT.memberwiseMult(reference - actorPosition);
	double distanceScore = DISTANCE_FACTOR * distance.lengthSquared();
	
	const double TIME_FACTOR = 100;
	double time = FrameTimer::current().frameTime() - value.second.time;
	double timeScore = -TIME_FACTOR * (time * time);
	
	const double DISPLACEMENT_FACTOR = 5;
	const double VELOCITY_FACTOR = 10;

	Vector2D displacement = ASPECT.memberwiseMult(actorPosition - value.second.position);
	Vector2D velocity = ASPECT.memberwiseMult(actor->getVelocity() - value.second.velocity);
	
	double displacementScore = -DISPLACEMENT_FACTOR * displacement.lengthSquared();
	double velocityScore = -VELOCITY_FACTOR * velocity.lengthSquared();
	
	return distanceScore + timeScore + displacementScore + velocityScore;
}

// A pairing of actor data and score.
struct User::ActorScore {
	Actor *actor;
	double score;
	User::ActorData data;
};

// A comparator for the priority queue.
struct User::ActorScoreComparator {
	// Less than comparison
	bool operator()(const User::ActorScore& score1, const User::ActorScore& score2)
	{
		return (score1.score < score2.score);
	}
};

void User::sendNetworkUpdate(const Actor *userActor)
{
	const double MAX_DISTANCE = 60;
	const Vector2D position = userActor->getPosition();

	WorldInstance::ActorList actors;
	ServerWorldInstance::current().getNearbyActors(actors, position);
	
	// Packet data
	std::vector< CreationUpdate > creation;
	std::vector< DestructionUpdate > destruction;
	std::vector< MovementUpdate > movement;
	
	// Heap data
	std::vector< ActorScore > scoreVector;
	
	// Replacement for _packetMap (swapped in later)
	PacketMap replacementMap;
	
	// Iterate for creation/movement
	BOOST_FOREACH(Actor *actor, actors) {

		// Skip the user.
		if (userActor == actor)
			continue;
		
		// Skip anyone too far away
		Rect offsetRect = actor->getCollisionRect() - position;
		if (std::abs(offsetRect.left) > MAX_DISTANCE ||
			std::abs(offsetRect.top) > MAX_DISTANCE ||
			std::abs(offsetRect.right) > MAX_DISTANCE ||
			std::abs(offsetRect.bottom) > MAX_DISTANCE) {
			
			continue;
		}

		
		// Not found; create it
		// TODO: Skip adding far-away actors.
		PacketMap::const_iterator found = _packetMap.find(actor);
		if (found == _packetMap.end()) {
			CreationUpdate create;
			create.id = actor->id();
			create.offsetRect = offsetRect;
			create.velocity = actor->getVelocity();
			create.sprite = actor->getSpriteType();
			create.isClientPlayer = false;
            create.name = actor->getName();
			creation.push_back(create);

			// Store this entry in the replacement map
			ActorData data;
			data.id = create.id;
			data.position = actor->getPosition();
			data.velocity = create.velocity;
			data.time = FrameTimer::current().frameTime();
			replacementMap[actor] = data;
			continue;
		}

		// Found; score it for moving later on
		ActorScore actorScore;
		actorScore.actor = found->first;
		actorScore.score = score(*found, position);
		actorScore.data = found->second;
		scoreVector.push_back(actorScore);
		_packetMap.erase(found);
	}
	
	// Iterate remaining for destruction
	BOOST_FOREACH(const PacketMap::value_type& value, _packetMap) {
		DestructionUpdate destroy;
		destroy.id = value.second.id;
		destruction.push_back(destroy);
	}
	
	// Make a heap of the actor data
	ActorScoreComparator comparator;
	std::make_heap(scoreVector.begin(), scoreVector.end(), comparator);

	// Pop from the heap until we're full.
	const int MAX_MOVEMENT_PACKETS = 20;
	while (movement.size() < MAX_MOVEMENT_PACKETS && !scoreVector.empty()) {
		std::pop_heap(scoreVector.begin(), scoreVector.end(), comparator);
		const ActorScore& actorScore = scoreVector.back();
		
		// Add packet data
		MovementUpdate move;
		move.id = actorScore.actor->id();
		move.displacement = actorScore.actor->getPosition() - position;
		move.velocity = actorScore.actor->getVelocity();
		movement.push_back(move);
		
		// Add new map data
		ActorData data;
		data.id = move.id;
		data.position = actorScore.actor->getPosition();
		data.velocity = move.velocity;
		data.time = FrameTimer::current().frameTime();
		replacementMap[actorScore.actor] = data;
		
		scoreVector.pop_back();
	}
	
	// Add the rest not packeted to the replacement map
	BOOST_FOREACH(const ActorScore& actorScore, scoreVector) {
		replacementMap[actorScore.actor] = actorScore.data;
	}

	
	// Put the replacement map in place
	std::swap(_packetMap, replacementMap);
	
	// Send the packets
	if (!creation.empty() || !destruction.empty()) {
		CreationPacket createPacket(position,
			creation.begin(), creation.end(),
			destruction.begin(), destruction.end());
		NetworkServer::current().send(createPacket, *this);
	}

	if (!movement.empty()) {
 		MovementPacket movePacket(position,
			movement.begin(), movement.end());
		NetworkServer::current().send(movePacket, *this);
	}
}

// Chebyshev distance
// 3 3 3 3 3 3 3
// 3 2 2 2 2 2 3
// 3 2 1 1 1 2 3
// 3 2 1 0 1 2 3
// 3 2 1 1 1 2 3
// 3 2 2 2 2 2 3
// 3 3 3 3 3 3 3
// Send/update everything within a Chebyshev distance of 1
// Forget everything past a Chebyshev distance of 3
// Client keeps everything within a Chebyshev distance of 4
void User::sendWorldMapUpdate(const Actor *userActor)
{
	ServerWorldMap& map = ServerWorldInstance::current().getWorldMap();
	const IVector2D tilePosition = IVector2D(userActor->getPosition());
	const IVector2D cellPosition = toCellCoordinates(tilePosition);
	
	// Forget far away stuff
	typedef std::vector<IVector2D> CellList;
	CellList iterList;
	BOOST_FOREACH(const CellMap::value_type& value, _cell) {
		iterList.push_back(value.first);
	}
	BOOST_FOREACH(const IVector2D& index, iterList) {
		const IVector2D dist = index - cellPosition;
		int32_t distance = std::max(std::abs(dist.x), std::abs(dist.y));
		if (distance > 3) {
			_cell.erase(index);
		}
	}

	// Send/update stuff within a chebyshev distance of 1
	typedef std::vector<CellUpdate> CellUpdateList;
	typedef std::vector<TileUpdate> TileUpdateList;
	CellUpdateList cellUpdates;
	TileUpdateList tileUpdates;
	IVector2D index;
	double now = FrameTimer::current().frameTime();
	for (index.y = cellPosition.y - 1; index.y <= cellPosition.y + 1; ++index.y) {
		for (index.x = cellPosition.x - 1; index.x <= cellPosition.x + 1; ++index.x) {
			std::pair<CellMap::iterator, bool> result = _cell.insert(
				CellMap::value_type(index, 0));
			if (result.second || map.getUpdateTime(index) > result.first->second) {
				cellUpdates.push_back(CellUpdate());
				cellUpdates.back().position = index - cellPosition;
				map.saveCell(index, cellUpdates.back().tiles);
			}
			result.first->second = now;
		}
	}

	// Send out the packet, if there's anything to send
	if (cellUpdates.size() > 0 || tileUpdates.size() > 0) {
		CellPacket packet(tilePosition,
			cellUpdates.begin(), cellUpdates.end(),
			tileUpdates.begin(), tileUpdates.end());
		NetworkServer::current().send(packet, *this);
	}
}
