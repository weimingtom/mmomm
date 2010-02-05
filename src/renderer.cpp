#include "renderer.h"

Renderer *Renderer::_current = 0;

Renderer::Renderer(const Vector2D& screenDimensions, bool fullscreen)
{
	this->_screenDimensions = screenDimensions;
    this->_fullscreen = fullscreen;
}

Renderer::~Renderer()
{
}

void Renderer::drawImage(Image *img, const Vector2D& position)
{
    return;
}

//Note: This code is mainly the same as the regular draw. It just adds a cut from the image.
void Renderer::drawClippedImage(Image *img, const Vector2D& position, const SDL_Rect& clip)
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
