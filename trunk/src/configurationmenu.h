#ifndef CONFIGURATIONMENU_H_
#define CONFIGURATIONMENU_H_

#include <iostream>
#include <guichan.hpp>
#include "gui.h"

using namespace std;

class ConfigurationMenu
{
private:
    gcn::Window      *_window;
    gcn::CheckBox    *_fullscreenCheckBox;
    gcn::Label       *_screenResolutionLabel;
    Gui              *_associatedGui;

public:
    ConfigurationMenu(Gui *associatedGui);
    ~ConfigurationMenu();
};

#endif
