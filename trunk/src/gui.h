#ifndef GUI_H_
#define GUI_H_

#include <iostream>
#include <guichan.hpp>
#include <guichan/sdl.hpp>
#include <guichan/opengl.hpp>
#include <guichan/opengl/openglsdlimageloader.hpp>
#include "SDL.h"

using namespace std;

class Gui
{
private:
    gcn::SDLInput              *_input;
    gcn::OpenGLGraphics        *_graphics;
    gcn::OpenGLSDLImageLoader  *_imageLoader;

    gcn::Gui        *_gui;
    gcn::Container  *_top;
    gcn::ImageFont  *_font;
    gcn::Label      *_label;
    gcn::Window     *_window;

public:
    Gui(SDL_Surface *screen);
    ~Gui();

    void pushInput(SDL_Event event);
    void logic();
    void draw();
};

#endif
