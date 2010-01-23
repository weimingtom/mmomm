#include "renderer.h"
#include "image.h"

int main(int argc, char **argv)
{
    if ( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0 ) {
        cout << "Unable to init SDL: " << SDL_GetError() << endl;
        exit(1);
    }

    renderer  = new Renderer(800, 600);
    Image *img = new Image("testimage.png");
    bool loop = true;

    while(loop)
    {
        SDL_Event event;

        SDL_WaitEvent(&event);

        switch (event.type) {
            case SDL_QUIT:
                loop = false;
                break;
        }

        renderer->drawImage(img, 0, 0);
        renderer->swapBuffers();
    }

    delete img;
    delete renderer;

    return 0;
}
