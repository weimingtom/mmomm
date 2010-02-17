#include "collisionPackets.h"
#include "frameTimer.h"
#include "networkServer.h"
#include "playerActor.h"
#include "serializationException.h"
#include "serverWorldInstance.h"
#include "user.h"
#include <iostream>

void MovementPacket::respondServer() const
{
	PlayerActor *actor = ServerWorldInstance::current().getUserActor(sender());
	if (!actor) {
		SERIALIZATION_EXCEPTION(
			"Received a movement packet from " <<
			sender().username() << " when no actor is assigned.");
	}
	BOOST_FOREACH(const MovementUpdate& update, movement()) {
		if (update.id != actor->id()) {
			SERIALIZATION_EXCEPTION(
				"Received a movement packet from " <<
				sender().username() << " for a non-player actor " << update.id << ".");
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

