#include "chatWindow.h"

ChatWindow *ChatWindow::_current = 0;

ChatWindow::ChatWindow(int x, int y)
{
    stringstream ss;

    try {
        _window = new gcn::Window("Chat");
        _window->setPosition(x, y);

        _chatBox = new gcn::TextBox();
        _chatBox->setPosition(0, 0);
        _chatBox->setSize(150, 50);
        _window->add(_chatBox);

        _inputLabel = new gcn::Label("Input");
        _inputLabel->setPosition(0, _chatBox->getY() + _chatBox->getHeight() + 5);
        _window->add(_inputLabel);

        ss.clear();
        ss.str("");
        ss << "Hello World!";

        _inputField = new gcn::TextField(ss.str());
        _inputField->setPosition(_inputLabel->getX() + _inputLabel->getWidth()  + 5,
                                 _inputLabel->getY() - 2);
        _inputField->setWidth(280);
        _window->add(_inputField);

        ss.clear();
        ss.str("");
        ss << Renderer::current().getScreenHeight();

        _window->resizeToContent();
        _chatBox->setWidth(_window->getWidth());
        _window->setVisible(true);

        Gui::current().addWidget(_window);

    } catch (gcn::Exception e) {
        cout << e.getMessage() << endl;
    }
}

ChatWindow::~ChatWindow()
{
    delete _inputField;
    delete _chatBox;
    delete _inputLabel;
    delete _window;
}

int ChatWindow::getX() const
{
    return _window->getX();
}

int ChatWindow::getY() const
{
    return _window->getY();
}
