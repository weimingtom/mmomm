#ifndef GUI_H_
#define GUI_H_

#include <iostream>
#include <assert.h>
#include <guichan.hpp>
#include <guichan/sdl.hpp>
#include <guichan/opengl.hpp>
#include <guichan/opengl/openglsdlimageloader.hpp>
#include "SDL.h"

using namespace std;

class Gui
{
private:
    static Gui *_current;

    gcn::SDLInput       *_input;
    gcn::Graphics       *_graphics;
    gcn::ImageLoader    *_imageLoader;

    gcn::Gui        *_gui;
    gcn::Container  *_top;
    gcn::ImageFont  *_font;

public:
    Gui(SDL_Surface *screen, bool softwareRendering);
    ~Gui();

    static Gui& current() { assert(_current); return *_current; }
    void setCurrent(Gui *current) { _current = current; }

    void pushInput(SDL_Event event);
    void addWidget(gcn::Widget *widget);
    void removeWidget(gcn::Widget *widget);
    void logic();
    void draw();
    void setTarget(SDL_Surface *screen, bool softwareRendering);
};

#endif