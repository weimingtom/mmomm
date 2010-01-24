#include "configurationmenu.h"

ConfigurationMenu::ConfigurationMenu(Gui *associatedGui)
{
    try {
        _associatedGui = associatedGui;
        _window = new gcn::Window();
        _window->setPosition(200, 200);

        _fullscreenCheckBox = new gcn::CheckBox("Fullscreen");
        _fullscreenCheckBox->adjustSize();
        _window->add(_fullscreenCheckBox);

        //_screenResolution = GET_RES_HERE
        _screenResolutionLabel = new gcn::Label("1024 x 768");
        _screenResolutionLabel->setPosition(0, 20);
        _window->add(_screenResolutionLabel);

        _window->resizeToContent();

        _window->setVisible(true);

        _associatedGui->addWidget(_window);

    } catch (gcn::Exception e) {
        cout << e.getMessage() << endl;
    }
}

ConfigurationMenu::~ConfigurationMenu()
{
    _window->remove(_fullscreenCheckBox);
    _window->remove(_screenResolutionLabel);
    _associatedGui->removeWidget(_window);
    delete _fullscreenCheckBox;
    delete _screenResolutionLabel;
    delete _window;
}
