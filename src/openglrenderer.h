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

class OpenGLRenderer;

#include "renderer.h"
#include "image.h"

using namespace std;

class OpenGLRenderer : public Renderer
{
private:
    SDL_Surface *_screen;
    GLint        _internalFormatRGB;
    GLint        _internalFormatRGBA;

    bool initScreen();

public:
    OpenGLRenderer(int screenWidth, int screenHeight, bool fullscreen);
    ~OpenGLRenderer();

    void drawImage(Image *img, float x, float y);
    void drawClippedImage(Image *img, float x, float y, SDL_Rect clip);
    void beginDraw();
    void swapBuffers();
    void generateTexture(GLuint *textureId, GLenum *textureFormat, SDL_Surface *surface);
    void deleteTexture(GLuint *textureId);

    SDL_Surface* getScreen();
    bool isSoftwareRenderer() { return false; }
};

#endif
