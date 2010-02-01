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

        bool alreadyDestroyed = false;
        for ( std::vector< int >::iterator i = ClientWorldInstance::current().GetErroneousDestructionIds().begin();
              i != ClientWorldInstance::current().GetErroneousDestructionIds().end(); i++ ) {
            if ( *i == create.id ) {
                alreadyDestroyed = true;
                ClientWorldInstance::current().GetErroneousDestructionIds().erase(i);
                break;
            }
        }
        if ( alreadyDestroyed )
            continue;

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
        else {
            ClientWorldInstance::current().GetErroneousDestructionIds().push_back(destruct.id);
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
