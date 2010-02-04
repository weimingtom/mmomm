#include "chatWindow.h"

ChatWindow *ChatWindow::_current = 0;

ChatWindow::ChatWindow(int x, int y)
{
    try {
        _window             = new gcn::Window("Chat");
        _tabArea            = new MMOMMTabbedArea;
        _serverTab          = new gcn::Tab();
        _generalTab         = new gcn::Tab();
        _privateTab         = new gcn::Tab();
        _serverContainer    = new gcn::Container();
        _generalContainer   = new gcn::Container();
        _privateContainer   = new gcn::Container();
        _wasteContainer     = new gcn::Container();
        _chatBox            = new gcn::TextBox();
        _chatBoxScroll      = new gcn::ScrollArea(_chatBox, gcn::ScrollArea::SHOW_NEVER, gcn::ScrollArea::SHOW_ALWAYS);
        _inputLabel         = new gcn::Label("Input");
        _targetLabel        = new gcn::Label("Target");
        _inputListener      = new textInputListener();
        _inputField         = new gcn::TextField("");
        _targetField        = new gcn::TextField("");


        _chatBox    ->setEditable(false);
        _inputField ->addActionListener(_inputListener);
        _serverTab  ->setCaption("Server");
        _generalTab ->setCaption("General");
        _privateTab ->setCaption("Private");


        _window             ->setSize(400, 200);
        _tabArea            ->setSize(_window->getWidth(), _window->getHeight());
        _generalContainer   ->setSize(_tabArea->getWidth()-5, _tabArea->getHeight()-5);
        _privateContainer   ->setSize(_tabArea->getWidth()-5, _tabArea->getHeight()-5);
        _serverContainer    ->setSize(_tabArea->getWidth()-5, _tabArea->getHeight()-5);
        _chatBoxScroll      ->setSize(_generalContainer->getWidth(), 50);
        _chatBox            ->setSize(_chatBoxScroll->getWidth(), 50);


        _window      ->setPosition(x, y);
        _inputLabel  ->setPosition(0, _chatBoxScroll->getY() + _chatBoxScroll->getHeight() + 5);
        _inputField  ->setPosition(_inputLabel->getX() + _inputLabel->getWidth()  + 5,
                                  _inputLabel->getY() - 2);
        _targetLabel ->setPosition(0, _inputLabel->getY() + _inputLabel->getHeight() + 5);
        _targetField ->setPosition(_targetLabel->getX() + _targetLabel->getWidth()  + 5,
                                   _targetLabel->getY() - 2);


        _inputField  ->setWidth(_tabArea->getWidth() - _inputLabel ->getWidth());
        _targetField ->setWidth(_tabArea->getWidth() - _targetLabel->getWidth());


        _generalContainer->add(_chatBoxScroll);
        _generalContainer->add(_inputLabel);
        _generalContainer->add(_inputField);

        _privateContainer->add(_chatBoxScroll);
        _privateContainer->add(_inputLabel);
        _privateContainer->add(_inputField);
        _privateContainer->add(_targetLabel);
        _privateContainer->add(_targetField);

        _serverContainer ->add(_chatBoxScroll);

        _wasteContainer  ->add(_inputField);
        _wasteContainer  ->add(_targetField);

        _tabArea->addTab(_serverTab,  _serverContainer);
        _tabArea->addTab(_generalTab, _generalContainer);
        _tabArea->addTab(_privateTab, _privateContainer);


        //_tabArea->setSelectedTab(_privateTab);

        _window->add(_tabArea);
        _window->setVisible(true);

        Gui::current().addWidget(_window);

    } catch (gcn::Exception e) {
        cout << e.getMessage() << endl;
    }
}

ChatWindow::~ChatWindow()
{
    delete _tabArea;
    delete _generalContainer;
    delete _privateContainer;
    delete _serverContainer;
    delete _generalTab;
    delete _privateTab;
    delete _serverTab;
    delete _chatBox;
    delete _chatBoxScroll;
    delete _inputLabel;
    delete _inputListener;
    delete _inputField;
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

void ChatWindow::addText(string text, ChatMessagePacket::TYPE type)
{
    bool scroll = false;
    bool active = (getType() == type) ? true : false;
    if(active) {
        if(_chatBoxScroll->getVerticalScrollAmount() == _chatBoxScroll->getVerticalMaxScroll())
            scroll = true;
        _chatBox->setText(_chatBox->getText() + text + "\n");
        if(scroll)
            _chatBoxScroll->setVerticalScrollAmount(_chatBoxScroll->getVerticalMaxScroll());
    }
    if(type == ChatMessagePacket::CHAT_MESSAGE_GENERAL)
        _generalText += text;
    else if(type == ChatMessagePacket::CHAT_MESSAGE_PRIVATE)
        _privateText += text;
    else if(type == ChatMessagePacket::CHAT_MESSAGE_SERVER)
        _serverText += text;
}

void ChatWindow::setText(string text)
{
    _chatBox->setText(text + "\n");
}

void ChatWindow::switchTabs()
{
    if(_tabArea->getSelectedTab() == _generalTab) {
        _type = ChatMessagePacket::CHAT_MESSAGE_GENERAL;
        _inputField->setEnabled(true);
        setText(_generalText);
    } else if(_tabArea->getSelectedTab() == _privateTab) {
        _type = ChatMessagePacket::CHAT_MESSAGE_PRIVATE;
        _targetField->setEnabled(true);
        setText(_privateText);
    }  else if(_tabArea->getSelectedTab() == _serverTab) {
        _type = ChatMessagePacket::CHAT_MESSAGE_SERVER;
        setText(_serverText);
    }
    _inputField->setText("");
}
