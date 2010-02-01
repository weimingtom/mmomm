#include "nativePackets.h"
#include "user.h"
#include "playerActor.h"
#include "serverWorldInstance.h"
#include "collisionPackets.h"
#include "clientAnimations.h"
#include "networkServer.h"
#include <iostream>

void ConnectionPacket::respondServer() const
{
	std::cout << "connected " << sender().username() << std::endl;

    const ServerWorldInstance::UserMap& users = ServerWorldInstance::current().GetUserMap();
    PlayerActor* actor = new PlayerActor(sender(), Rect(1.0 * users.size(), 0.0, 1.0, 1.0));

    // Tell existing users about the new actor
    CreationUpdate update;
    update.id = actor->id();
    update.rect = actor->GetCollisionRect();
    update.velocity = Vector2D(0, 0);
    update.animation = ClientAnimations::FIGHTER;

    CreationPacket::CreationList creationList;
    creationList.push_back(update);
    CreationPacket::DestructionList destructionList;

    for ( ServerWorldInstance::UserMap::const_iterator i = users.begin(); i != users.end(); i++ ) {
        CreationPacket packet(creationList.begin(), creationList.end(), destructionList.begin(), destructionList.end());
        NetworkServer::current().send(packet, *(i->first));
    }

    // Tell the new user about existing actors
    const WorldInstance::ActorMap& actors = WorldInstance::current().GetActorMap();
    CreationPacket::CreationList existingList;
    for ( WorldInstance::ActorMap::const_iterator i = actors.begin(); i != actors.end(); i++ ) {
        CreationUpdate update;
        update.id = i->first;
        update.rect = i->second->GetCollisionRect();
        update.velocity = Vector2D(0, 0);
        update.animation = ClientAnimations::FIGHTER;
        existingList.push_back(update);
    }
    CreationPacket packet(existingList.begin(), existingList.end(), destructionList.begin(), destructionList.end());
    NetworkServer::current().send(packet, sender());

    ServerWorldInstance::current().AddUser(sender(), actor);
}

void DisconnectionPacket::respondServer() const
{
	std::cout << "disconnected " << sender().username() << std::endl;

    DestructionUpdate update;
    update.id = ServerWorldInstance::current().GetUserActor(sender())->id();

    delete ServerWorldInstance::current().GetUserActor(sender());
    ServerWorldInstance::current().RemoveUser(sender());

    CreationPacket::CreationList creationList;
    CreationPacket::DestructionList destructionList;
    destructionList.push_back(update);

    const ServerWorldInstance::UserMap& users = ServerWorldInstance::current().GetUserMap();
    for ( ServerWorldInstance::UserMap::const_iterator i = users.begin(); i != users.end(); i++ ) {
        CreationPacket packet(creationList.begin(), creationList.end(), destructionList.begin(), destructionList.end());
        NetworkServer::current().send(packet, *(i->first));
    }
}

void TamperPacket::respondServer() const
{
	std::cout << "tamper " << sender().username() << std::endl;
	
}
