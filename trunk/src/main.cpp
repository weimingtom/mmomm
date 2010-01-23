#include "renderer.h"
#include "image.h"
#include "gui.h"

int main(int argc, char **argv)
{
    if ( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0 ) {
        cout << "Unable to init SDL: " << SDL_GetError() << endl;
        exit(1);
    }

    SDL_EnableUNICODE(1);
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

    renderer  = new Renderer(800, 600);
    Image *img = new Image("testimage.png");
    Gui *gui = new Gui(renderer->getScreen());
    bool loop = true;

    while(loop)
    {
        SDL_Event event;

        while(SDL_PollEvent(&event)) {

            switch (event.type) {
                case SDL_QUIT:
                    loop = false;
                    break;
            }

            gui->pushInput(event);
        }

        gui->logic();


        renderer->drawImage(img, 0, 0);
        gui->draw();
        renderer->swapBuffers();


    }

    delete img;
    delete renderer;

    return 0;
}
