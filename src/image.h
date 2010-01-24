#ifndef IMAGE_H_
#define IMAGE_H_

class Image;

#ifdef WIN32
#include <windows.h>
#endif
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "renderer.h"

using namespace std;

class Image
{
private:
    SDL_Surface *_surface;
    GLuint textureId;
    GLenum textureFormat;
    int width;
    int height;

    public:
    /**
    * Constructor of Image.
    * @param filename the name of the file to load
    */
    Image(std::string filename);
    ~Image();
    /**
    * Gets the image surface
    */
    SDL_Surface* getSurface() { return _surface; }
    GLuint getTextureId() { return textureId; }
    void setTextureFormat(GLenum textureFormat) { this->textureFormat = textureFormat; }
    GLenum getTextureFormat() { return textureFormat; }
    int getWidth() { return width; }
    int getHeight() { return height; }
};

#endif
