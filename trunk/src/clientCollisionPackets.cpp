#include "collisionPackets.h"
#include "clientWorldInstance.h"
#include "playerClientActor.h"
#include "simulatedClientActor.h"
#include <iostream>

void CreationPacket::respondClient() const
{
	BOOST_FOREACH(const CreationUpdate& create, creation()) {
		WorldInstance::ActorMap::const_iterator found =
			ClientWorldInstance::current().getActorMap().find(create.id);
		if (found != ClientWorldInstance::current().getActorMap().end()) {
			delete found->second;
		}

        bool alreadyDestroyed = false;
        for ( std::vector< int >::iterator i = ClientWorldInstance::current().getErroneousDestructionIds().begin();
              i != ClientWorldInstance::current().getErroneousDestructionIds().end(); i++ ) {
            if ( *i == create.id ) {
                alreadyDestroyed = true;
                ClientWorldInstance::current().getErroneousDestructionIds().erase(i);
                break;
            }
        }
        if ( alreadyDestroyed )
            continue;
		
		ClientActor *actor;
		if (create.isClientPlayer) {
			actor = new PlayerClientActor(
				create.id, referencePoint() + create.offsetRect, create.velocity,
				ClientSprites::SpriteType(create.sprite));
            ClientWorldInstance::current().setClientPlayerActor(actor);
		}
		else {
			actor = new SimulatedClientActor(
				create.id, referencePoint() + create.offsetRect, create.velocity,
				ClientSprites::SpriteType(create.sprite));
		}
		actor->setName(create.name);
	}
	BOOST_FOREACH(const DestructionUpdate& destruct, destruction()) {
		WorldInstance::ActorMap::const_iterator found =
			ClientWorldInstance::current().getActorMap().find(destruct.id);
		if (found != ClientWorldInstance::current().getActorMap().end()) {
			delete found->second;
		}
        else {
            ClientWorldInstance::current().getErroneousDestructionIds().push_back(destruct.id);
        }
	}
}

void MovementPacket::respondClient() const
{
	BOOST_FOREACH(const MovementUpdate& move, movement()) {
		WorldInstance::ActorMap::const_iterator found =
			ClientWorldInstance::current().getActorMap().find(move.id);
		if (found != ClientWorldInstance::current().getActorMap().end()) {
			found->second->interpolate(timestamp(), referencePoint() + move.displacement, move.velocity);
		}
	}
}
