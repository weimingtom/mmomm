#include "softwarerenderer.h"
#include <math.h>

SoftwareRenderer::SoftwareRenderer(int screenWidth, int screenHeight) : Renderer(screenWidth, screenHeight)
{
    this->_screenWidth = screenWidth;
    this->_screenHeight = screenHeight;

    _screen = SDL_SetVideoMode(_screenWidth, _screenHeight, 0, SDL_SWSURFACE);
    if(_screen == NULL) {
        //logger->echoError("SDL_SetVideoMode failed: " + std::string(SDL_GetError()));
        cout << "SDL_SetVideoMode failed: " << std::string(SDL_GetError()) << endl;
        //TODO throw exception
    }
}

void SoftwareRenderer::drawImage(Image *img, float x, float y)
{
    if(img == NULL)
        return;

    //the x and the y coordinates are only accepted as an SDL_Rect
    SDL_Rect offset;
    offset.x = Sint16(x);
    offset.y = Sint16(y);

    //put image on screen
    if(SDL_BlitSurface(img->getSurface(), NULL, _screen, &offset) == -1)
        cout << "Blitting of image failed" << endl;
}

//Note: This code is mainly the same as the regular draw. It just adds a cut from the image.
void SoftwareRenderer::drawClippedImage(Image *img, float x, float y, SDL_Rect clip)
{
    if(img == NULL)
        return;

    //the x and the y coordinates are only accepted as an SDL_Rect
    SDL_Rect offset;
    offset.x = Sint16(x);
    offset.y = Sint16(y);

    //put image on screen
    if(SDL_BlitSurface(img->getSurface(), &clip, _screen, &offset) == -1)
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
