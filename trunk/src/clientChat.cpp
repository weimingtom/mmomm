#include "chat.h"
#include "chatWindow.h"
#include <iostream>

void ChatMessagePacket::respondClient() const
{
    if(type() == CHAT_MESSAGE_PRIVATE)
    {
        string newmsg = from();
        newmsg += ": ";
        newmsg += message();
	    ChatWindow::current().addText(newmsg, type());
    }
    else
    {
        string newmsg = from();
        newmsg += ": ";
        newmsg += message();
        ChatWindow::current().addText(newmsg, type());
    }
}
