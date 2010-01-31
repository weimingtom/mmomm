#include "image.h"
#include <sstream>
#include <iostream>

Image::~Image()
{
#ifndef NDEBUG
    cout << _filename << " destroyed" << endl;
#endif
    destroy();
}

Image::Image(string filename)
:   _filename(filename)
{
    load();
}

void Image::reload()
{
    destroy();
    load();
}

void Image::load()
{
    SDL_Surface *loadedImage = NULL;
    loadedImage = IMG_Load(_filename.c_str());

    if (!loadedImage)
    {
        //logger->echoError("Could not load image: " + string(IMG_GetError()));
        cout << "Could not load image: " << string(IMG_GetError()) << endl;
        exit(1);
    }

    SDL_Surface *surface = SDL_DisplayFormatAlpha(loadedImage);
    SDL_FreeSurface(loadedImage);
    if (!surface)
    {
        /*logger->echoError("Could not convert image to display pixel format: " +
                          string(SDL_GetError()));*/
        cout << "Could not convert image to display pixel format: " << string(SDL_GetError()) << endl;
        exit(1);
    }

    _width  = surface->w;
    _height = surface->h;

    Renderer::current().generateTexture(&_textureId, &_textureFormat, surface);
    if(Renderer::current().isSoftwareRenderer())
        _surface = surface;
    else
    {
        SDL_FreeSurface(surface);
        _surface = NULL;
    }
}

void Image::destroy()
{
    Renderer::current().deleteTexture(&_textureId);
    if(_surface != NULL)
    {
        SDL_FreeSurface(_surface);
        _surface = 0;
    }
}

