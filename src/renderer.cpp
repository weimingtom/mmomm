#include "renderer.h"
#include <GL/glext.h>
#include <boost/scoped_array.hpp>

Renderer *renderer = NULL;

//ret_type (APIENTRY * glExtensionName)(parameters) = NULL;
GLenum extension = GL_TEXTURE_RECTANGLE_ARB;

/*int nextPowerOfTwo(int k)
{
  k--;
  for (unsigned int i=1; i<sizeof(k)*8; i=i*2)
          k = k | k >> i;
  logger->echoDebug("Nexthigher: " + k);
  return k+1;
}*/

//blatantly stolen from Box2D
inline unsigned nextPowerOfTwo(unsigned x)
{
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >> 16);
    return x + 1;
}

bool isPower2(int x)
{
	return ((x > 0) && ((x & (x - 1)) == 0));
}

std::string translateGLError(GLenum errorcode)
{
    std::string errorStr = (char*)gluErrorString(errorcode);
    return errorStr;
}

Renderer::Renderer(int screenWidth, int screenHeight)
{
    this->_screenWidth  = screenWidth;
    this->_screenHeight = screenHeight;
    if(!initScreen())
        cout << "Failed to initialise _screen" << endl;
    //logger->echoError("Failed to initialise _screen");

    //TODO throw exception here
}

bool Renderer::initScreen()
{
    GLenum error;
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    _screen = SDL_SetVideoMode(_screenWidth, _screenHeight, 0, SDL_OPENGL);
    if(_screen == NULL) {
        //logger->echoError("SDL_SetVideoMode failed: " + std::string(SDL_GetError()));
        cout << "SDL_SetVideoMode failed: " << std::string(SDL_GetError()) << endl;
        return false;
    }

    char *extensions = (char *)glGetString(GL_EXTENSIONS);

    //enable easier NPOT textures.
    if(!strstr(extensions, "ARB_texture_rectangle")) {
        //logger->echoDebug("Graphics card does not support GL_ARB_texture_rectangle");
        cout << "Graphics card does not support GL_ARB_texture_rectangle" << endl;
        extension = GL_TEXTURE_2D;
    }

    if(!strstr(extensions, "ARB_texture_compression")) {
        //logger->echoDebug("Using compression");
        cout << "Using compression" << endl;
        _internalFormatRGB = GL_COMPRESSED_RGB_ARB;
        _internalFormatRGBA = GL_COMPRESSED_RGBA_ARB;
    }
    else {
        //logger->echoDebug("No compression available");
        _internalFormatRGB = GL_RGB;
        _internalFormatRGBA = GL_RGBA;
    }

    glEnable(extension);

    //which colour the _screen gets cleared to(black in this case)
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    //set the opengl window to our window size
    glViewport(0, 0, _screenWidth, _screenHeight);

    //clear the _screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //clear the Projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //load perspective
    glOrtho(0.0f, _screenWidth, _screenHeight, 0.0f, -1.0f, 1.0f);

    //clear the Modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if((error = glGetError()) != GL_NO_ERROR) {
        //logger->echoError("GLError happened: " + translateGLError(error));
        cout << "GLError happened: " << translateGLError(error) << endl;
        return false;
    }

    //logger->echoDebug("Renderer initialised _screen successful");
    cout << "Renderer initialised _screen successful" << endl;
    return true;
}

void Renderer::drawImage(Image *img, float x, float y)
{
    GLenum error;

    glEnable(extension);

    glBindTexture(extension, img->getTextureId());

    if(extension == GL_TEXTURE_2D) {
        glBegin(GL_QUADS);
          //Top-left vertex (corner)
          glTexCoord2f(0, 0); //image/texture
          glVertex2f(x, y); //_screen coordinates

          //Bottom-left vertex (corner)
          glTexCoord2f(1, 0);
          glVertex2f(x+img->getWidth(), y);

          //Bottom-right vertex (corner)
          glTexCoord2f(1, 1);
          glVertex2f(x+img->getWidth(), y+img->getHeight());

          //Top-right vertex (corner)
          glTexCoord2f(0, 1);
          glVertex2f(x, y+img->getHeight());
        glEnd();
    }
    else {
        glBegin(GL_QUADS);
          //Top-left vertex (corner)
          glTexCoord2f(0, 0); //image/texture
          glVertex2f(x, y); //_screen coordinates

          //Bottom-left vertex (corner)
          glTexCoord2f(GLfloat(img->getWidth()), 0);
          glVertex2f(GLfloat(x+img->getWidth()), y);

          //Bottom-right vertex (corner)
          glTexCoord2f(GLfloat(img->getWidth()), GLfloat(img->getHeight()));
          glVertex2f(x+GLfloat(img->getWidth()), y+GLfloat(img->getHeight()));

          //Top-right vertex (corner)
          glTexCoord2f(0, GLfloat(img->getHeight()));
          glVertex2f(x, y+GLfloat(img->getHeight()));
        glEnd();
    }

    if((error = glGetError()) != GL_NO_ERROR)
        //logger->echoError("A gl error happened in drawImage: " + translateGLError(error));
        cout << "A GL error happened in drawImage: " << translateGLError(error) << endl;

    glDisable(extension);

    return;
}

//Note: This code is mainly the same as the regular draw. It just adds a cut from the image.
void Renderer::drawClippedImage(Image *img, float x, float y, SDL_Rect clip)
{
    GLenum error;

    glEnable(extension);

    glBindTexture(extension, img->getTextureId());

    if(extension == GL_TEXTURE_2D) {
        float xp = (float)clip.x/(float)img->getWidth();
        float yp = (float)clip.y/(float)img->getHeight();
        float wp = (float)clip.w/(float)img->getWidth();
        float hp = (float)clip.h/(float)img->getHeight();
        glBegin(GL_QUADS);
          //Top-left vertex (corner)
          glTexCoord2f(xp, yp); //image/texture
          glVertex2i((int)x, (int)y); //_screen coordinates

          //Bottom-left vertex (corner)
          glTexCoord2f(xp+wp, yp);
          glVertex2i((int)x+(int)clip.w, (int)y);

          //Bottom-right vertex (corner)
          glTexCoord2f(xp+wp, yp+hp);
          glVertex2i((int)x+(int)clip.w, (int)y+clip.h);

          //Top-right vertex (corner)
          glTexCoord2f(xp, yp+hp);
          glVertex2i((int)x, (int)y+(int)clip.h);
        glEnd();
    }
    else { //extension = GL_TEXTURE_RECTANGLE_ARB
        glBegin(GL_QUADS);
          //Top-left vertex (corner)
          glTexCoord2f(clip.x, clip.y); //image/texture
          glVertex2i((int)x, (int)y); //_screen coordinates

          //Bottom-left vertex (corner)
          glTexCoord2f(GLfloat(clip.x + clip.w), GLfloat(clip.y));
          glVertex2i((int)x+(int)clip.w, (int)y);

          //Bottom-right vertex (corner)
          glTexCoord2f(GLfloat(clip.x + clip.w), GLfloat(clip.y + clip.h));
          glVertex2i((int)x+(int)clip.w, (int)y+(int)clip.h);

          //Top-right vertex (corner)
          glTexCoord2f(GLfloat(clip.x), GLfloat(clip.y + clip.h));
          glVertex2i((int)x, (int)y+(int)clip.h);
        glEnd();
    }

    if((error = glGetError()) != GL_NO_ERROR)
        cout << "A GL error happened in drawClippedImage: " << translateGLError(error) << endl;

    glDisable(extension);

    return;
}

void Renderer::beginDraw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::swapBuffers()
{
    glFlush();
    SDL_GL_SwapBuffers();
}

//convert to _screen format?
void Renderer::generateTexture(GLuint *textureId, GLenum *textureFormat, SDL_Surface *surface)
{
    GLenum  error;
    GLint   internalFormat;
    int     numberOfColors = surface->format->BytesPerPixel;


    if (numberOfColors == 4) {      // contains an alpha channel
        if (surface->format->Rmask == 0x000000ff)
            *textureFormat = GL_RGBA;
        else
            *textureFormat = GL_BGRA;
        internalFormat = _internalFormatRGBA;
    }
    else if (numberOfColors == 3) { // no alpha channel
        if (surface->format->Rmask == 0x000000ff)
            *textureFormat = GL_RGB;
        else
            *textureFormat = GL_BGR;
        internalFormat = _internalFormatRGB;
    }
    else {
        //logger->echoError("Image is not made out of 3 colours or 3 colours and an alpha channel(4). Cannot generate texture.");
        cout << "Image is not made out of 3 colours or 3 colours and an alpha channel(4). Cannot generate texture." << endl;
        return;
    }

    glGenTextures(1, textureId);
    glEnable(extension);
    glBindTexture(extension, *textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if(extension == GL_TEXTURE_2D) {
        if(isPower2(surface->w) && isPower2(surface->h)) {
          //logger->echoDebug("POT texture detected");
          glTexImage2D(extension, 0, internalFormat /*numberOfColors*/, surface->w, surface->h, 0,
                       *textureFormat, GL_UNSIGNED_BYTE, surface->pixels);

        }
        else {
			int w = nextPowerOfTwo(surface->w);
			int h = nextPowerOfTwo(surface->h);
			int size = w * h;
			boost::scoped_array<unsigned> imageStore(new unsigned[size]);
            GLvoid *scaledImage = imageStore.get();
            GLint ret = gluScaleImage(*textureFormat, surface->w, surface->h, GL_UNSIGNED_BYTE, surface->pixels,
                        w, h, GL_UNSIGNED_BYTE, scaledImage);
            if(ret != 0)
            {
                //logger->echoError("A gl error happened in gluScaleImage: " + translateGLError(glGetError()));
                cout << "A gl error happened in gluScaleImage: " << translateGLError(glGetError()) << endl;
                return;
            }

            glTexImage2D(extension, 0, internalFormat, w, h, 0,
                        *textureFormat, GL_UNSIGNED_BYTE, scaledImage);
        }


    }
    else {
        glTexImage2D(extension, 0, internalFormat, surface->w, surface->h, 0,
                    *textureFormat, GL_UNSIGNED_BYTE, surface->pixels);
    }

    if((error = glGetError()) != GL_NO_ERROR)
        //logger->echoError("A gl error happened in gl(Sub)TexImage2D: " + translateGLError(error));
        cout << "A GL error happened in generateTexture: " << translateGLError(error) << endl;

    glDisable(extension);
}

void Renderer::deleteTexture(GLuint *textureId)
{
    glDeleteTextures(1, textureId);
}

SDL_Surface* Renderer::getScreen()
{
    return _screen;
}
