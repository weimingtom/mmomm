#include "loginmenu.h"

LoginMenu *LoginMenu::_current = 0;

LoginMenu::LoginMenu(int x, int y)
{
    stringstream ss;

    try {
        _okButtonListener = new okButtonListener();
        _optionsButtonListener = new optionsButtonListener();

        _window = new gcn::Window();
        _window->setPosition(x, y);

        _createAccountCheckBox = new gcn::CheckBox("Create New Account");
        _createAccountCheckBox->adjustSize();
        _createAccountCheckBox->setPosition(0, 80);
        _window->add(_createAccountCheckBox);

        _hostLabel = new gcn::Label("Host");
        _hostLabel->setPosition(0, 0);
        _window->add(_hostLabel);

        _portLabel = new gcn::Label("Port");
        _portLabel->setPosition(0, 20);
        _window->add(_portLabel);

        _usernameLabel = new gcn::Label("Username");
        _usernameLabel->setPosition(0, 40);
        _window->add(_usernameLabel);

        _passwordLabel = new gcn::Label("Password");
        _passwordLabel->setPosition(0, 60);
        _window->add(_passwordLabel);

        int xPos = 5 + max(max(_hostLabel->getX() + _hostLabel->getWidth(),
                               _portLabel->getX() + _portLabel->getWidth()),
                           max(_usernameLabel->getX() + _usernameLabel->getWidth(),
                               _passwordLabel->getX() + _passwordLabel->getWidth()));

        _hostField = new gcn::TextField("localhost");
        _hostField->setPosition(xPos, _hostLabel->getY() - 2);
        _hostField->setWidth(80);
        _window->add(_hostField);

        _portField = new gcn::TextField("33033");
        _portField->setPosition(xPos, _portLabel->getY() - 2);
        _portField->setWidth(80);
        _window->add(_portField);

        _usernameField = new gcn::TextField("hangar");
        _usernameField->setPosition(xPos, _usernameLabel->getY() - 2);
        _usernameField->setWidth(80);
        _window->add(_usernameField);

        _passwordField = new gcn::TextField("awesome");
        _passwordField->setPosition(xPos, _passwordLabel->getY() - 2);
        _passwordField->setWidth(80);
        _window->add(_passwordField);

        _okButton = new gcn::Button("Ok");
        _okButton->setPosition(0, 100);
        _okButton->addActionListener(_okButtonListener);
        _window->add(_okButton);

        _optionsButton = new gcn::Button("Options");
        _optionsButton->setPosition(30, 100);
        _optionsButton->addActionListener(_optionsButtonListener);
        _window->add(_optionsButton);

        _window->resizeToContent();
        _window->setVisible(true);

        Gui::current().addWidget(_window);

    } catch (gcn::Exception e) {
        cout << e.getMessage() << endl;
    }
}

LoginMenu::~LoginMenu()
{
    delete _createAccountCheckBox;
    delete _hostLabel;
    delete _portLabel;
    delete _usernameLabel;
    delete _passwordLabel;
    delete _hostField;
    delete _portField;
    delete _usernameField;
    delete _passwordField;
    delete _okButton;
    delete _optionsButton;
    delete _window;
}

int LoginMenu::getX() const
{
    return _window->getX();
}

int LoginMenu::getY() const
{
    return _window->getY();
}

void LoginMenu::okButtonListener::action(const gcn::ActionEvent& actionEvent)
{
    SDL_Event  event;
    LoginData* data = new LoginData();

    stringstream ss;
    ss.clear();
    ss.str("");
    ss << LoginMenu::current()._portField->getText();
    ss >> data->port;

    data->host          = LoginMenu::current()._hostField->getText();
    data->username      = LoginMenu::current()._usernameField->getText();
    data->password      = LoginMenu::current()._passwordField->getText();

    data->createAccount = LoginMenu::current()._createAccountCheckBox->isSelected();

    event.type  = SDL_USEREVENT;
    event.user.code  = EVENT_LOGIN_OK;
    event.user.data1 = data;

    SDL_PushEvent(&event);
}

void LoginMenu::optionsButtonListener::action(const gcn::ActionEvent& actionEvent)
{
    SDL_Event event;
    event.type  = SDL_USEREVENT;
    event.user.code  = EVENT_LOGIN_OPTIONS;

    SDL_PushEvent(&event);
}