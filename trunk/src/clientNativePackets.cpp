#include "nativePackets.h"
#include "chatWindow.h"
#include "clientWorldInstance.h"
#include <iostream>

void ConnectionPacket::respondClient() const
{
    ChatWindow::current().addText( "Connected" );
}

void DisconnectionPacket::respondClient() const
{
    ChatWindow::current().addText( "Disconnected: " + reason() );
	
	delete &ClientWorldInstance::current();
	ClientWorldInstance::setCurrent(new ClientWorldInstance());
	
}

void TamperPacket::respondClient() const
{
	ChatWindow::current().addText( "Tampered" );
}
