#include "configurationmenu.h"

#include <string>
#include <sstream>
#include <vector>

ConfigurationMenu *ConfigurationMenu::_current = 0;

ConfigurationMenu::ConfigurationMenu()
{
    stringstream ss;
    string str;
    ss.clear();
    ss.str("");

    ss << Renderer::current().getScreenWidth();
    ss << " x ";
    ss << Renderer::current().getScreenHeight();

    str = ss.str();

    try {
        _okButtonListener = new okButtonListener();

        _window = new gcn::Window();
        _window->setPosition(200, 200);

        _fullscreenCheckBox = new gcn::CheckBox("Fullscreen");
        _fullscreenCheckBox->adjustSize();
        _window->add(_fullscreenCheckBox);

        _screenResolutionLabel = new gcn::Label(str);
        _screenResolutionLabel->setPosition(0, 20);
        _window->add(_screenResolutionLabel);

        _okButton = new gcn::Button("Ok");
        _okButton->setPosition(0, 50);
        _okButton->addActionListener(_okButtonListener);
        _window->add(_okButton);

        _cancelButton = new gcn::Button("Cancel");
        _cancelButton->setPosition(30, 50);
        _window->add(_cancelButton);

        _window->resizeToContent();
        _window->setVisible(true);

        Gui::current().addWidget(_window);

    } catch (gcn::Exception e) {
        cout << e.getMessage() << endl;
    }
}

ConfigurationMenu::~ConfigurationMenu()
{
    _window->remove(_fullscreenCheckBox);
    _window->remove(_screenResolutionLabel);

    Gui::current().removeWidget(_window);

    delete _fullscreenCheckBox;
    delete _screenResolutionLabel;
    delete _okButton;
    delete _cancelButton;
    delete _okButtonListener;
    delete _window;
}
