#include "nativePackets.h"
#include "user.h"
#include "playerActor.h"
#include "serverWorldInstance.h"
#include "collisionPackets.h"
#include "clientSprites.h"
#include "networkServer.h"
#include <iostream>

void ConnectionPacket::respondServer() const
{
	std::cout << "connected " << sender().username() << std::endl;

    const ServerWorldInstance::UserMap& users = ServerWorldInstance::current().getUserMap();
    PlayerActor* actor = new PlayerActor(sender(), Rect(0.25 + 1.0 * users.size(), 0.0, 0.75 + 1.0 * users.size(), 1.0));
    actor->setName(sender().username());

    // Tell the user about the new actor; others will get it later
    CreationUpdate update;
    update.id = actor->id();
    update.offsetRect = actor->getCollisionRect() - actor->getPosition();
    update.velocity = actor->getVelocity();
    update.sprite = actor->getSpriteType();
    update.isClientPlayer = true;
    update.name = actor->getName();

    CreationPacket::CreationList creationList;
    creationList.push_back(update);
    CreationPacket::DestructionList destructionList;

    CreationPacket packet(actor->getPosition(),
		creationList.begin(), creationList.end(),
		destructionList.begin(), destructionList.end());
    NetworkServer::current().send(packet, sender());

    ServerWorldInstance::current().addUser(sender(), actor);
}

void DisconnectionPacket::respondServer() const
{
	std::cout << "disconnected " << sender().username() << std::endl;

    delete ServerWorldInstance::current().getUserActor(sender());
    ServerWorldInstance::current().removeUser(sender());
}

void TamperPacket::respondServer() const
{
	std::cout << "tamper " << sender().username() << std::endl;
}
