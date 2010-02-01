#include "nativePackets.h"
#include "chatWindow.h"
#include <iostream>

void ConnectionPacket::respondClient() const
{
    ChatWindow::current().addText( "Connected." );
}

void DisconnectionPacket::respondClient() const
{
    ChatWindow::current().addText( "Disconnected." );
}

void TamperPacket::respondClient() const
{
	ChatWindow::current().addText( "Tampered." );
}
