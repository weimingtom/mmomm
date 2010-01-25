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
#include <assert.h>

class Renderer;

#include "image.h"

using namespace std;

class Renderer
{
private:
    static Renderer *_current;

protected:
    int _screenHeight;
    int _screenWidth;
    bool _fullscreen;

public:
    Renderer(int screenWidth, int screenHeight, bool fullscreen);
    virtual ~Renderer();

    static Renderer& current() { assert(_current); return *_current; }

    //Delete the current before calling this function.
    void setCurrent(Renderer *renderer) { _current = renderer; }

    virtual void drawImage(Image *img, float x, float y);
    virtual void drawClippedImage(Image *img, float x, float y, SDL_Rect clip);
    virtual void beginDraw();
    virtual void swapBuffers();
    virtual void generateTexture(GLuint *textureId, GLenum *textureFormat, SDL_Surface *surface);
    virtual void deleteTexture(GLuint *textureId);

    int getScreenWidth() { return _screenWidth; }
    int getScreenHeight() { return _screenHeight; }

    virtual SDL_Surface* getScreen();
    virtual bool isSoftwareRenderer();
};

#endif
