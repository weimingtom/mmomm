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

    const ServerWorldInstance::UserMap& users = ServerWorldInstance::current().GetUserMap();
    PlayerActor* actor = new PlayerActor(sender(), Rect(0.25 + 1.0 * users.size(), 0.0, 0.75 + 1.0 * users.size(), 1.0));
    actor->SetName(sender().username());

    // Tell the user about the new actor; others will get it later
    CreationUpdate update;
    update.id = actor->id();
    update.offsetRect = actor->GetCollisionRect() - actor->GetPosition();
    update.velocity = actor->GetVelocity();
    update.sprite = actor->GetSpriteType();
    update.isClientPlayer = true;
    update.name = actor->GetName();

    CreationPacket::CreationList creationList;
    creationList.push_back(update);
    CreationPacket::DestructionList destructionList;

    CreationPacket packet(actor->GetPosition(),
		creationList.begin(), creationList.end(),
		destructionList.begin(), destructionList.end());
    NetworkServer::current().send(packet, sender());

    ServerWorldInstance::current().AddUser(sender(), actor);
}

void DisconnectionPacket::respondServer() const
{
	std::cout << "disconnected " << sender().username() << std::endl;

    delete ServerWorldInstance::current().GetUserActor(sender());
    ServerWorldInstance::current().RemoveUser(sender());
}

void TamperPacket::respondServer() const
{
	std::cout << "tamper " << sender().username() << std::endl;
}
