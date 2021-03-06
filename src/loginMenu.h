#ifndef LOGINMENU_H_
#define LOGINMENU_H_

#include <cassert>
#include <guichan.hpp>

using namespace std;

class LoginMenu
{
public:
    LoginMenu(int x, int y);
    ~LoginMenu();

    int getX() const;
    int getY() const;

    static LoginMenu& current() { assert(_current); return *_current; }
    static void setCurrent(LoginMenu *current) { _current = current; }

    struct LoginData {
        string host;
        int    port;
        string username;
        string password;
        bool   createAccount;
    };

private:
    class okButtonListener : public gcn::ActionListener
    {
        void action(const gcn::ActionEvent& actionEvent);
    };

    class localhostButtonListener : public gcn::ActionListener
    {
        void action(const gcn::ActionEvent& actionEvent);
    };

    class optionsButtonListener : public gcn::ActionListener
    {
        void action(const gcn::ActionEvent& actionEvent);
    };

    gcn::Window             *_window;
    gcn::CheckBox           *_createAccountCheckBox;
    gcn::Label              *_hostLabel;
    gcn::Label              *_portLabel;
    gcn::Label              *_usernameLabel;
    gcn::Label              *_passwordLabel;

    gcn::Button             *_okButton;
    gcn::Button             *_localhostButton;
    gcn::Button             *_optionsButton;

    gcn::TextField          *_hostField;
    gcn::TextField          *_portField;
    gcn::TextField          *_usernameField;
    gcn::TextField          *_passwordField;

    okButtonListener        *_okButtonListener;
    localhostButtonListener *_localhostButtonListener;
    optionsButtonListener   *_optionsButtonListener;

    static LoginMenu        *_current;

};

#endif
