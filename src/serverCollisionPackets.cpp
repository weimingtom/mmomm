#include "collisionPackets.h"
#include "frameTimer.h"
#include "networkServer.h"
#include "playerActor.h"
#include "serverWorldInstance.h"
#include "user.h"
#include <iostream>

void MovementPacket::respondServer() const
{
	PlayerActor *actor = ServerWorldInstance::current().GetUserActor(sender());
	if (!actor) {
		std::cout << "Received a movement packet for ";
		std::cout << sender().username() << " when no actor is assigned." << std::endl;
	}
	BOOST_FOREACH(const MovementUpdate& update, movement()) {
		if (update.id != actor->id()) {
			std::cout << "Received a movement update for ";
			std::cout << sender().username() << " for non-player actor." << std::endl;
			continue;
		}
		if (actor->getUpdateTime() < timestamp()) {
			actor->setUpdateTime(timestamp());
			double time = FrameTimer::current().frameTime() - timestamp();
			Vector2D position = update.position + update.velocity * time;
			actor->SetPosition(position);
			actor->SetVelocity(update.velocity);
		}
	}
}

