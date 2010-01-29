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
    SDL_Surface     *_surface;
    string           _filename;
    GLuint           _textureId;
    GLenum           _textureFormat;
    int              _width;
    int              _height;

    void load();
    void destroy();

public:
    Image(string filename);
    ~Image();

    void reload();

    SDL_Surface* getSurface()                   { return _surface; }
    GLuint getTextureId()                       { return _textureId; }
    void setTextureFormat(GLenum textureFormat) { this->_textureFormat = textureFormat; }
    GLenum getTextureFormat()                   { return _textureFormat; }
    int getWidth()                              { return _width; }
    int getHeight()                             { return _height; }
    string getFilename()                        { return _filename; }
};

#endif
