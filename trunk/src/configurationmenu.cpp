#include "configurationmenu.h"

ConfigurationMenu *ConfigurationMenu::_current = 0;

ConfigurationMenu::ConfigurationMenu()
{
    stringstream ss;

    try {
        _okButtonListener = new okButtonListener();

        _window = new gcn::Window();
        _window->setPosition(200, 200);

        _fullscreenCheckBox = new gcn::CheckBox("Fullscreen");
        _fullscreenCheckBox->adjustSize();
        if(Renderer::current().isFullscreen())
            _fullscreenCheckBox->setSelected(true);
        _window->add(_fullscreenCheckBox);

        _screenResolutionLabel = new gcn::Label("Resolution");
        _screenResolutionLabel->setPosition(0, 20);
        _window->add(_screenResolutionLabel);

        ss.clear();
        ss.str("");
        ss << Renderer::current().getScreenWidth();

        _widthField = new gcn::TextField(ss.str());
        _widthField->setPosition(_screenResolutionLabel->getX() + _screenResolutionLabel->getWidth()  + 5,
                                 _screenResolutionLabel->getY() - 2);
        _widthField->setWidth(40);
        _window->add(_widthField);

        ss.clear();
        ss.str("");
        ss << Renderer::current().getScreenHeight();

        _heightField = new gcn::TextField(ss.str());
        _heightField->setPosition(_widthField->getX() + _widthField->getWidth()  + 5,
                                  _widthField->getY());
        _heightField->setWidth(40);
        _window->add(_heightField);

        _rendererCheckBox = new gcn::CheckBox("Software Renderer");
        _rendererCheckBox->adjustSize();
        _rendererCheckBox->setPosition(0, 40);
        if(Renderer::current().isSoftwareRenderer())
            _rendererCheckBox->setSelected(true);
        _window->add(_rendererCheckBox);

        _okButton = new gcn::Button("Ok");
        _okButton->setPosition(0, 60);
        _okButton->addActionListener(_okButtonListener);
        _window->add(_okButton);

        _cancelButton = new gcn::Button("Cancel");
        _cancelButton->setPosition(30, 60);
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
/*    _window->remove(_fullscreenCheckBox);
    _window->remove(_screenResolutionLabel);
    _window->remove(_widthField);
    _window->remove(_heightField);
    _window->remove(_okButton);
    _window->remove(_cancelButton);
    */

    //Gui::current().removeWidget(_window);

    delete _fullscreenCheckBox;
    delete _screenResolutionLabel;
    delete _widthField;
    delete _heightField;
    delete _okButton;
    delete _cancelButton;
    delete _okButtonListener;
    delete _window;
}