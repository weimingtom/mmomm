#include "image.h"
#include <sstream>
#include <iostream>

Image::~Image()
{
    renderer->deleteTexture(&textureId);
}

Image::Image(std::string filename)
{
  SDL_Surface *loadedImage = NULL;
  loadedImage = IMG_Load(filename.c_str());

  if (!loadedImage)
  {
    //logger->echoError("Could not load image: " + std::string(IMG_GetError()));
    cout << "Could not load image: " << std::string(IMG_GetError()) << endl;
    exit(1);
  }

  SDL_Surface *surface = SDL_DisplayFormatAlpha(loadedImage);
  SDL_FreeSurface(loadedImage);
  if (!surface)
  {
    /*logger->echoError("Could not convert image to display pixel format: " +
                      std::string(SDL_GetError()));*/
    cout << "Could not convert image to display pixel format: " << std::string(SDL_GetError()) << endl;
    exit(1);
  }

  width  = surface->w;
  height = surface->h;

  renderer->generateTexture(&textureId, &textureFormat, surface);
  SDL_FreeSurface(surface);
}
