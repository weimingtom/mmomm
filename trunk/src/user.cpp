#include "user.h"
#include "actor.h"
#include "collisionPackets.h"
#include "frameTimer.h"
#include "networkServer.h"
#include "serverWorldInstance.h"
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
