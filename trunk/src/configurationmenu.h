#ifndef CONFIGURATIONMENU_H_
#define CONFIGURATIONMENU_H_

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <guichan.hpp>

#include "gui.h"
#include "renderer.h"

using namespace std;

class okButtonListener;

class ConfigurationMenu
{
    friend class okButtonListener;

private:
    gcn::Window         *_window;
    gcn::CheckBox       *_fullscreenCheckBox;
    gcn::CheckBox       *_rendererCheckBox;
    gcn::Label          *_screenResolutionLabel;

    gcn::Button         *_okButton;
    gcn::Button         *_cancelButton;

    gcn::TextField      *_widthField;
    gcn::TextField      *_heightField;

    okButtonListener    *_okButtonListener;

    static ConfigurationMenu *_current;

public:
    ConfigurationMenu();
    ~ConfigurationMenu();

    static ConfigurationMenu& current() { assert(_current); return *_current; }
    void setCurrent(ConfigurationMenu *current) { _current = current; }
};

class okButtonListener : public gcn::ActionListener
{
    void action(const gcn::ActionEvent& actionEvent)
    {
        SDL_Event event;
        stringstream ss;
        int *ints = new int[4];

        ss.clear();
        ss.str("");

        ss << ConfigurationMenu::current()._widthField->getText();
        ss >> ints[0];

        ss.clear();
        ss.str("");

        ss << ConfigurationMenu::current()._heightField->getText();
        ss >> ints[1];

        if(ConfigurationMenu::current()._fullscreenCheckBox->isSelected())
            ints[2] = 1;
        else
            ints[2] = 0;

        if(ConfigurationMenu::current()._rendererCheckBox->isSelected())
            ints[3] = 1;
        else
            ints[3] = 0;

        event.type  = SDL_USEREVENT;
        event.user.code  = 0; //TODO We need some defines for this.
        event.user.data1 = ints;

        SDL_PushEvent(&event);
    }

};

#endif
