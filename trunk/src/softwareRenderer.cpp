#include "softwareRenderer.h"
#include <math.h>
#include <string>
#include <sstream>
#include <iostream>

SoftwareRenderer::SoftwareRenderer(const Vector2D& screenDimensions, bool fullscreen) : Renderer(screenDimensions, fullscreen)
{
    unsigned flags;

    if(fullscreen)
        flags = SDL_FULLSCREEN | SDL_SWSURFACE;
    else
        flags = SDL_SWSURFACE;

    _screen = SDL_SetVideoMode(int(screenDimensions.x), int(screenDimensions.y), 0, flags);
    if(_screen == NULL) {
        //logger->echoError("SDL_SetVideoMode failed: " + std::string(SDL_GetError()));
        cout << "SDL_SetVideoMode failed: " << std::string(SDL_GetError()) << endl;
        //TODO throw exception
    }
}

SoftwareRenderer::~SoftwareRenderer()
{
    SDL_FreeSurface(_screen);
}

void SoftwareRenderer::drawImage(Image *img, const Vector2D& position)
{
    if(img == NULL)
        return;

    //the x and the y coordinates are only accepted as an SDL_Rect
    SDL_Rect offset;
    offset.x = Sint16(position.x);
    offset.y = Sint16(position.y);

    //put image on screen
    if(SDL_BlitSurface(img->getSurface(), NULL, _screen, &offset) == -1)
        cout << "Blitting of image failed" << endl;
}

void SoftwareRenderer::drawSurface(SDL_Surface *surface, const Vector2D& position)
{
    if(surface == NULL)
        return;

    //the x and the y coordinates are only accepted as an SDL_Rect
    SDL_Rect offset;
    offset.x = Sint16(position.x);
    offset.y = Sint16(position.y);

    //put image on screen
    if(SDL_BlitSurface(surface, NULL, _screen, &offset) == -1)
        cout << "Blitting of surface failed" << endl;
}

//Note: This code is mainly the same as the regular draw. It just adds a cut from the image.
void SoftwareRenderer::drawClippedImage(Image *img, const Vector2D& position, const SDL_Rect& clip)
{
    if(img == NULL)
        return;

    //the x and the y coordinates are only accepted as an SDL_Rect
    SDL_Rect offset;
    offset.x = Sint16(position.x);
    offset.y = Sint16(position.y);

	// NOTE: const_cast should be ok in this situation
    //put image on screen
    if(SDL_BlitSurface(img->getSurface(), const_cast<SDL_Rect *>(&clip), _screen, &offset) == -1)
        cout << "Blitting of image failed" << endl;
}

void SoftwareRenderer::beginDraw()
{
    SDL_FillRect(_screen, NULL, 0x00000000);
}

void SoftwareRenderer::swapBuffers()
{
    SDL_Flip(_screen);
}

//convert to _screen format?
void SoftwareRenderer::generateTexture(GLuint *textureId, GLenum *textureFormat, SDL_Surface *surface)
{
    return;
}

void SoftwareRenderer::deleteTexture(GLuint *textureId)
{
    return;
}

SDL_Surface* SoftwareRenderer::getScreen()
{
    return _screen;
}
