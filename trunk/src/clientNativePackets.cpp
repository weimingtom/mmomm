#include "nativePackets.h"
#include "chatWindow.h"
#include "clientWorldInstance.h"
#include <iostream>

void ConnectionPacket::respondClient() const
{
    ChatWindow::current().addText("Connected", ChatMessagePacket::CHAT_MESSAGE_SERVER);
}

void DisconnectionPacket::respondClient() const
{
    ChatWindow::current().addText("Disconnected: " + reason(), ChatMessagePacket::CHAT_MESSAGE_SERVER);

	delete &ClientWorldInstance::current();
	ClientWorldInstance::setCurrent(new ClientWorldInstance());

}

void TamperPacket::respondClient() const
{
	ChatWindow::current().addText("Tampered", ChatMessagePacket::CHAT_MESSAGE_SERVER);
}
