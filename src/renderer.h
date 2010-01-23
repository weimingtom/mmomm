#ifndef OPENGL_RENDERER_H_
#define OPENGL_RENDERER_H_

#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include <sstream>
#include <iostream>

class Renderer;

#include "image.h"

using namespace std;

class Renderer
{
private:
    SDL_Surface *_screen;
    int          _screenHeight;
    int          _screenWidth;
    GLint        _internalFormatRGB;
    GLint        _internalFormatRGBA;

    bool initScreen();

public:
    Renderer(int screenWidth, int screenHeight);

    void drawImage(Image *img, float x, float y);
    void drawClippedImage(Image *img, float x, float y, SDL_Rect clip);
    void swapBuffers();
    void generateTexture(GLuint *textureId, GLenum *textureFormat, SDL_Surface *surface);
    void deleteTexture(GLuint *textureId);

    SDL_Surface* getScreen();
};

extern Renderer *renderer;

#endif
