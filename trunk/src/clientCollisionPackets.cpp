#include "collisionPackets.h"
#include "clientActor.h"
#include "clientWorldInstance.h"
#include <iostream>

void CreationPacket::respondClient() const
{
	BOOST_FOREACH(const CreationUpdate& create, creation()) {
		WorldInstance::ActorMap::const_iterator found =
			ClientWorldInstance::current().GetActorMap().find(create.id);
		if (found != ClientWorldInstance::current().GetActorMap().end()) {
			delete found->second;
		}

		ClientActor *actor = new ClientActor(create.id, create.rect, create.velocity,
			ClientSprites::SpriteType(create.sprite));
        if ( create.isClientPlayer )
            ClientWorldInstance::current().SetClientPlayerActor(actor);
	}
	BOOST_FOREACH(const DestructionUpdate& destruct, destruction()) {
		WorldInstance::ActorMap::const_iterator found =
			ClientWorldInstance::current().GetActorMap().find(destruct.id);
		if (found != ClientWorldInstance::current().GetActorMap().end()) {
			delete found->second;
		}
	}
}

void MovementPacket::respondClient() const
{
	BOOST_FOREACH(const MovementUpdate& move, movement()) {
		WorldInstance::ActorMap::const_iterator found =
			ClientWorldInstance::current().GetActorMap().find(move.id);
		if (found != ClientWorldInstance::current().GetActorMap().end()) {
			found->second->interpolate(timestamp(), move.position, move.velocity);
		}
	}
}
