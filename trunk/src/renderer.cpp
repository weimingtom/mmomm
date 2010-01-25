#include "renderer.h"

Renderer *Renderer::_current = 0;

Renderer::Renderer(int screenWidth, int screenHeight, bool fullscreen)
{
    this->_screenWidth = screenWidth;
    this->_screenHeight = screenHeight;
    this->_fullscreen = fullscreen;
}

Renderer::~Renderer()
{
}

void Renderer::drawImage(Image *img, float x, float y)
{
    return;
}

//Note: This code is mainly the same as the regular draw. It just adds a cut from the image.
void Renderer::drawClippedImage(Image *img, float x, float y, SDL_Rect clip)
{
    return;
}

void Renderer::beginDraw()
{
    return;
}

void Renderer::swapBuffers()
{
    return;
}

//convert to _screen format?
void Renderer::generateTexture(GLuint *textureId, GLenum *textureFormat, SDL_Surface *surface)
{
    return;
}

void Renderer::deleteTexture(GLuint *textureId)
{
    return;
}

SDL_Surface* Renderer::getScreen()
{
    return NULL;
}

bool Renderer::isSoftwareRenderer()
{
    return false;
}
