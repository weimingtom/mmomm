#ifndef SOFTWARE_RENDERER_H_
#define SOFTWARE_RENDERER_H_

#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <string>
#include <sstream>
#include <iostream>

class SoftwareRenderer;

#include "image.h"
#include "renderer.h"

using namespace std;

class SoftwareRenderer : public Renderer
{
private:
    SDL_Surface *_screen;

public:
    SoftwareRenderer(int screenWidth, int screenHeight);

    void drawImage(Image *img, float x, float y);
    void drawClippedImage(Image *img, float x, float y, SDL_Rect clip);
    void beginDraw();
    void swapBuffers();
    void generateTexture(GLuint *textureId, GLenum *textureFormat, SDL_Surface *surface);
    void deleteTexture(GLuint *textureId);

    SDL_Surface* getScreen();
    bool isSoftwareRenderer() { return true; }
};

#endif

