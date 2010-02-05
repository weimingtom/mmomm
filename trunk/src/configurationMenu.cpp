#include "configurationMenu.h"

ConfigurationMenu *ConfigurationMenu::_current = 0;

ConfigurationMenu::ConfigurationMenu(int x, int y)
{
    stringstream ss;

    try {
        _okButtonListener = new okButtonListener();
        _cancelButtonListener = new cancelButtonListener();

        _window = new gcn::Window("Configuration");
        _window->setPosition(x, y);

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
        ss << Renderer::current().getScreenDimensions().x;

        _widthField = new gcn::TextField(ss.str());
        _widthField->setPosition(_screenResolutionLabel->getX() + _screenResolutionLabel->getWidth()  + 5,
                                 _screenResolutionLabel->getY() - 2);
        _widthField->setWidth(40);
        _window->add(_widthField);

        ss.clear();
        ss.str("");
        ss << Renderer::current().getScreenDimensions().y;

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
        _cancelButton->addActionListener(_cancelButtonListener);
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
    delete _fullscreenCheckBox;
    delete _screenResolutionLabel;
    delete _widthField;
    delete _heightField;
    delete _okButton;
    delete _cancelButton;
    delete _okButtonListener;
    delete _cancelButtonListener;
    delete _window;
}

int ConfigurationMenu::getX() const
{
    return _window->getX();
}

int ConfigurationMenu::getY() const
{
    return _window->getY();
}
