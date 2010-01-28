#include "ChatWindow.h"

ChatWindow *ChatWindow::_current = 0;

ChatWindow::ChatWindow(int x, int y)
{
    stringstream ss;

    try {
        _window = new gcn::Window();
        _window->setPosition(x, y);

        _chatLabel = new gcn::Label("Chat");
        _chatLabel->setPosition(0, 0);
        _window->add(_chatLabel);

        ss.clear();
        ss.str("");
        ss << "Hello World!";

        _chatField = new gcn::TextField(ss.str());
        _chatField->setPosition(_chatLabel->getX() + _chatLabel->getWidth()  + 5,
                                 _chatLabel->getY() - 2);
        _chatField->setWidth(280);
        _window->add(_chatField);

        ss.clear();
        ss.str("");
        ss << Renderer::current().getScreenHeight();

        _window->resizeToContent();
        _window->setVisible(true);

        Gui::current().addWidget(_window);

    } catch (gcn::Exception e) {
        cout << e.getMessage() << endl;
    }
}

ChatWindow::~ChatWindow()
{
/*    _window->remove(_fullscreenCheckBox);
    _window->remove(_screenResolutionLabel);
    _window->remove(_widthField);
    _window->remove(_heightField);
    _window->remove(_okButton);
    _window->remove(_cancelButton);
    */

    //Gui::current().removeWidget(_window);

    delete _chatField;
}

int ChatWindow::getX() const
{
    return _window->getX();
}

int ChatWindow::getY() const
{
    return _window->getY();
}