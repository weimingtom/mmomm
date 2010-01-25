#ifndef CONFIGURATIONMENU_H_
#define CONFIGURATIONMENU_H_

#include <iostream>
#include <guichan.hpp>

#include "gui.h"
#include "renderer.h"

using namespace std;

class okButtonListener;

class ConfigurationMenu
{
    friend class okButtonListener;

private:
    gcn::Window     *_window;
    gcn::CheckBox   *_fullscreenCheckBox;
    gcn::Label      *_screenResolutionLabel;

    gcn::Button     *_okButton;
    gcn::Button     *_cancelButton;

    okButtonListener *_okButtonListener;

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
        cout << "ok button clicked!" << endl;
        SDL_Event event;
        int *ints = new int[1];
        if(ConfigurationMenu::current()._fullscreenCheckBox->isSelected())
            ints[0] = 1;
        else
            ints[0] = 0;

        event.type  = SDL_USEREVENT;
        event.user.code  = 0; //We need some defines for this.
        event.user.data1 = ints;

        SDL_PushEvent(&event);
    }

};

#endif
