#ifndef RENDERER_H_
#define RENDERER_H_

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
protected:
    int _screenHeight;
    int _screenWidth;

public:
    Renderer(int screenWidth, int screenHeight);

    virtual void drawImage(Image *img, float x, float y);
    virtual void drawClippedImage(Image *img, float x, float y, SDL_Rect clip);
    virtual void beginDraw();
    virtual void swapBuffers();
    virtual void generateTexture(GLuint *textureId, GLenum *textureFormat, SDL_Surface *surface);
    virtual void deleteTexture(GLuint *textureId);

    int getScreenWidth() { return _screenWidth; };
    int getScreenHeight() { return _screenHeight; };

    virtual SDL_Surface* getScreen();
    virtual bool isSoftwareRenderer();
};

extern Renderer *renderer;

#endif
