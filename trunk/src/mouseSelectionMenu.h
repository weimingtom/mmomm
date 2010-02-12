#ifndef MOUSESELECTIONMENU_H_
#define MOUSESELECTIONMENU_H_

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <guichan.hpp>
#include <guichan/graphics.hpp>

#include "gui.h"

using namespace std;

class MouseSelectionMenu : public gcn::Window
{
public:
    MouseSelectionMenu(int x, int y);
    ~MouseSelectionMenu();

    void addLabel(gcn::Label *label);
    void removeLabel(gcn::Label *label);

    gcn::Rectangle getChildrenArea();

    void draw(gcn::Graphics* graphics);

    void mouseMoved(gcn::MouseEvent &mouseEvent);

private:
    gcn::Rectangle drawRect;
};

#endif
