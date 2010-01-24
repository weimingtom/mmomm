#ifndef GUI_H_
#define GUI_H_

#include <iostream>
#include <vector>
#include <guichan.hpp>
#include <guichan/sdl.hpp>
#include <guichan/opengl.hpp>
#include <guichan/opengl/openglsdlimageloader.hpp>
#include "SDL.h"

using namespace std;

class Gui
{
private:
    gcn::SDLInput       *_input;
    gcn::Graphics *_graphics;
    gcn::ImageLoader    *_imageLoader;

    gcn::Gui        *_gui;
    gcn::Container  *_top;
    gcn::ImageFont  *_font;

public:
    Gui(SDL_Surface *screen, bool softwareRendering);
    ~Gui();

    void pushInput(SDL_Event event);
    void addWidget(gcn::Widget *widget);
    void removeWidget(gcn::Widget *widget);
    void logic();
    void draw();
};

#endif
