#ifndef OPENGL_RENDERER_H_
#define OPENGL_RENDERER_H_

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
    OpenGLRenderer(const Vector2D& screenDimensions, bool fullscreen);
    ~OpenGLRenderer();

    void drawImage(Image *img, const Vector2D& position);
    void drawSurface(SDL_Surface *surface, const Vector2D& position);
    void drawClippedImage(Image *img, const Vector2D& position, const SDL_Rect& clip);
    void beginDraw();
    void swapBuffers();
    void generateTexture(GLuint *textureId, GLenum *textureFormat, SDL_Surface *surface);
    void deleteTexture(GLuint *textureId);

    SDL_Surface* getScreen();
    bool isSoftwareRenderer() { return false; }
};

#endif
