#include "gui.h"

Gui *Gui::_current = 0;

Gui::Gui(SDL_Surface *screen, bool softwareRendering)
{
    try {
        if(softwareRendering)
        {
            _imageLoader = new gcn::SDLImageLoader();
            gcn::Image::setImageLoader(_imageLoader); //DON'T remove this from before the graphics initialization. I warned you.

            _graphics = new gcn::SDLGraphics();
            ((gcn::SDLGraphics*)_graphics)->setTarget(screen);
        }
        else
        {
            _imageLoader = new gcn::OpenGLSDLImageLoader();
            gcn::Image::setImageLoader(_imageLoader);

            _graphics = new gcn::OpenGLGraphics(screen->w, screen->h);

        }

        //cout << "screen: " << screen->w << ", " << screen->h << endl;

        _input = new gcn::SDLInput();

        _font = new gcn::ImageFont("imagefont.png", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?+-/():;%&`'*#=[]\"<>{}^~|_@$\\");
        gcn::Widget::setGlobalFont(_font);

        _gui = new gcn::Gui();
        _gui->setGraphics(_graphics);
        _gui->setInput(_input);

        _top = new gcn::Container();
        _top->setDimension(gcn::Rectangle(0, 0, screen->w, screen->h));
        _top->setBaseColor(gcn::Color(0,0,0,0));
        _top->setOpaque(false);
        _gui->setTop(_top);

    } catch (gcn::Exception e) {
        cout << e.getMessage() << endl;
    }
}

Gui::~Gui()
{
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

void Gui::addWidget(gcn::Widget *widget)
{
    _top->add(widget);
}

void Gui::removeWidget(gcn::Widget *widget)
{
    _top->remove(widget);
}

void Gui::logic()
{
    try {
        _gui->logic();
    } catch (gcn::Exception e) {
        cout << e.getMessage() << endl;
    }
}

void Gui::draw()
{
    try {
        _gui->draw();
    } catch (gcn::Exception e) {
        cout << e.getMessage() << endl;
    }
}

void Gui::setTarget(SDL_Surface *screen, bool softwareRendering)
{
    try {
        delete _graphics;
        if(softwareRendering)
        {
            _graphics = new gcn::SDLGraphics();
            ((gcn::SDLGraphics*)_graphics)->setTarget(screen);
        }
        else
        {
            cout << "screen: " << screen << endl;
            _graphics = new gcn::OpenGLGraphics(screen->w, screen->h);
        }
        _gui->setGraphics(_graphics);
    } catch (gcn::Exception e) {
        cout << e.getMessage() << endl;
    }
}
