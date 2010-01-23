#include "gui.h"

Gui::Gui(SDL_Surface *screen)
{
    try {
        _imageLoader = new gcn::OpenGLSDLImageLoader();
        gcn::Image::setImageLoader(_imageLoader);

        _graphics = new gcn::OpenGLGraphics(screen->w, screen->h);
        //cout << "screen: " << screen->w << ", " << screen->h << endl;

        _input = new gcn::SDLInput();

        _font = new gcn::ImageFont("fixedfont.bmp", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
        gcn::Widget::setGlobalFont(_font);

        _gui = new gcn::Gui();
        _gui->setGraphics(_graphics);
        _gui->setInput(_input);

        _top = new gcn::Container();
        _top->setDimension(gcn::Rectangle(0, 0, screen->w, screen->h));
        _top->setOpaque(false);
        _gui->setTop(_top);

        cout << "Opaque: " << _top->isOpaque() << endl;

        _label = new gcn::Label("Hello World");
        _label->setPosition(280, 220);
        _top->add(_label);

        _window = new gcn::Window("Test window!");
        _window->setPosition(0, 0);
        _window->setSize(100, 100);
        //_window->setOpaque(false);
        _top->add(_window);
    } catch (gcn::Exception e) {
        cout << e.getMessage() << endl;
    }
}

Gui::~Gui()
{
    delete _window;
    delete _label;
    delete _font;
    delete _top;
    delete _gui;

    delete _input;
    delete _graphics;
    delete _imageLoader;

}


void Gui::pushInput(SDL_Event event)
{
    _input->pushInput(event);
}

void Gui::logic()
{
    _gui->logic();
}

void Gui::draw()
{
    _gui->draw();
}
