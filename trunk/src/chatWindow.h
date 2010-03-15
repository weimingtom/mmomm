#ifndef CHATWINDOW_H_
#define CHATWINDOW_H_

#include <cassert>
#include <string>
#include <guichan.hpp>

#include "networkClient.h"
#include "chat.h"

using namespace std;

class ChatWindow
{
public:
    ChatWindow(int x, int y);
    ~ChatWindow();

    int                         getX() const;
    int                         getY() const;

    static ChatWindow&          current() { assert(_current); return *_current; }
    static void                 setCurrent(ChatWindow *current) { _current = current; current->finishInit(); }

    void                        addText(string text, ChatMessagePacket::TYPE type);
    void                        setText(string text);
    void                        switchTabs();
    ChatMessagePacket::TYPE     getType() { return _type; }


private:
    class textInputListener : public gcn::ActionListener
    {
        void action(const gcn::ActionEvent& actionEvent)
        {
            if(NetworkClient::current().isActive())
            {
                ChatMessagePacket chatMessage(NetworkClient::current().username(),
                                              ChatWindow::current()._targetField->getText(),
                                              ChatWindow::current()._inputField->getText(),
                                              ChatWindow::current().getType());
                ChatWindow::current()._inputField->setText("");
                NetworkClient::current().send(chatMessage);
            }
        }
    };

    class tabSwitchListener : public gcn::ActionListener
    {
        void action(const gcn::ActionEvent& actionEvent)
        {
            ChatWindow::current().switchTabs();
        }
    };

    void finishInit();

    gcn::Window				 *_window;
    gcn::TabbedArea          *_tabArea;
    gcn::Tab                 *_serverTab;
    gcn::Tab                 *_generalTab;
    gcn::Tab                 *_privateTab;
    gcn::Container           *_serverContainer;
    gcn::Container           *_generalContainer;
    gcn::Container           *_privateContainer;
    gcn::TextField           *_inputField;
    gcn::TextField           *_targetField;
    gcn::ScrollArea          *_chatBoxScroll;
    gcn::TextBox             *_chatBox;
    gcn::Label               *_inputLabel;
    gcn::Label               *_targetLabel;

    textInputListener        *_inputListener;
    tabSwitchListener        *_switchListener;

    std::string               _serverText;
    std::string               _generalText;
    std::string               _privateText;

    ChatMessagePacket::TYPE   _type;

    static ChatWindow *_current;
};

#endif
