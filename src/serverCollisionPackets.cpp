#include "collisionPackets.h"
#include "frameTimer.h"
#include "networkServer.h"
#include "playerActor.h"
#include "serverWorldInstance.h"
#include "user.h"
#include <iostream>

void MovementPacket::respondServer() const
{
	PlayerActor *actor = ServerWorldInstance::current().getUserActor(sender());
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
			// TODO: Verify that the movement is feasible (avoid teleport hacks).

			actor->setUpdateTime(timestamp());
			double time       = FrameTimer::current().frameTime() - timestamp();
			Vector2D position = referencePoint() + update.displacement + update.velocity * time;
			Vector2D move     = position - actor->getPosition();
            Rect     r        = actor->getCollisionRect();

            actor->setSize(.5 * r.dim());
			actor->move(move);
            actor->setSize(r.dim());
			actor->setVelocity(update.velocity);
		}
	}
}

