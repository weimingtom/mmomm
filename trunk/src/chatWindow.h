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

using namespace std;

class ChatWindow
{
private:
	gcn::Window				 *_window;
    gcn::TextField           *_inputField;
    gcn::TextBox             *_chatBox;
    gcn::Label               *_inputLabel;

    static ChatWindow *_current;

public:
    ChatWindow(int x, int y);
    ~ChatWindow();

    int getX() const;
    int getY() const;

    static ChatWindow& current() { assert(_current); return *_current; }
    void setCurrent(ChatWindow *current) { _current = current; }

    void addText(string text) {_chatBox->setText(_chatBox->getText() + text + "\n");}
};

#endif
