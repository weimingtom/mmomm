#ifndef CHATWINDOW_H_
#define CHATWINDOW_H_

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <guichan.hpp>

#include "gui.h"
#include "renderer.h"
#include "events.h"
#include "chat.h"
#include "networkClient.h"

using namespace std;

class ChatWindow
{
public:
    ChatWindow(int x, int y);
    ~ChatWindow();

    int                         getX() const;
    int                         getY() const;

    static                      ChatWindow& current() { assert(_current); return *_current; }
    void                        setCurrent(ChatWindow *current) { _current = current; }

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

    class MMOMMTabbedArea : public gcn::TabbedArea
    {
        public:
        MMOMMTabbedArea() : gcn::TabbedArea() {}
        void mousePressed(gcn::MouseEvent& mouseEvent)
        {
            if (mouseEvent.isConsumed() && mouseEvent.getSource()->isFocusable())
                return;

            if (mouseEvent.getButton() == gcn::MouseEvent::LEFT)
            {
                gcn::Widget* widget = mTabContainer->getWidgetAt(mouseEvent.getX(), mouseEvent.getY());
                gcn::Tab* tab = dynamic_cast<gcn::Tab*>(widget);

                if (tab != NULL)
                {
                    setSelectedTab(tab);
                    ChatWindow::current().switchTabs();
                }
            }

            requestFocus();
        }

    };

    gcn::Window				 *_window;
    MMOMMTabbedArea          *_tabArea;
    gcn::Tab                 *_serverTab;
    gcn::Tab                 *_generalTab;
    gcn::Tab                 *_privateTab;
    gcn::Container           *_serverContainer;
    gcn::Container           *_generalContainer;
    gcn::Container           *_privateContainer;
    gcn::Container           *_wasteContainer; //Seriously, we need to fix guichan.
    gcn::TextField           *_inputField;
    gcn::TextField           *_targetField;
    gcn::ScrollArea          *_chatBoxScroll;
    gcn::TextBox             *_chatBox;
    gcn::Label               *_inputLabel;
    gcn::Label               *_targetLabel;

    textInputListener        *_inputListener;

    std::string               _serverText;
    std::string               _generalText;
    std::string               _privateText;

    ChatMessagePacket::TYPE   _type;

    static ChatWindow *_current;
};

#endif
