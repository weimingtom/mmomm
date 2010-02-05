#ifndef RENDERER_H_
#define RENDERER_H_

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef min
#undef max
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include <sstream>
#include <iostream>
#include <assert.h>
#include "rect.h"
#include "vector2D.h"

class Renderer;

#include "image.h"

using namespace std;

class Renderer
{
private:
    static Renderer *_current;

protected:
    Vector2D _screenDimensions;
    bool _fullscreen;

public:
    Renderer(const Vector2D& screenDimensions, bool fullscreen);
    virtual ~Renderer();

    static Renderer& current() { assert(_current); return *_current; }

    //Delete the current before calling this function.
    void setCurrent(Renderer *renderer) { _current = renderer; }

    virtual void drawImage(Image *img, const Vector2D& position);
    virtual void drawClippedImage(Image *img, const Vector2D& position, const SDL_Rect& clip);
    virtual void beginDraw();
    virtual void swapBuffers();
    virtual void generateTexture(GLuint *textureId, GLenum *textureFormat, SDL_Surface *surface);
    virtual void deleteTexture(GLuint *textureId);

	Vector2D getScreenDimensions() { return _screenDimensions; }
    
    virtual SDL_Surface* getScreen();
    virtual bool isSoftwareRenderer();

    bool isFullscreen() { return _fullscreen; };
};

#endif
