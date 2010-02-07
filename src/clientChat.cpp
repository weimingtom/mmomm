#include "chat.h"
#include "chatWindow.h"
#include <iostream>

void ChatMessagePacket::respondClient() const
{
    if(type() == CHAT_MESSAGE_PRIVATE) {
        if(target().compare(NetworkClient::current().username()) == 0) {
            string newmsg = from();
            newmsg += ": ";
            newmsg += message();
            ChatWindow::current().addText(newmsg, type());
        } else {
            string newmsg = "To ";
            newmsg += target();
            newmsg += ": ";
            newmsg += message();
            ChatWindow::current().addText(newmsg, type());
        }
    } else {
        string newmsg = from();
        newmsg += ": ";
        newmsg += message();
        ChatWindow::current().addText(newmsg, type());
    }
}
