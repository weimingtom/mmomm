#include "renderer.h"
#include "openglrenderer.h"
#include "softwarerenderer.h"
#include "image.h"
#include "gui.h"
#include "configurationmenu.h"

#include <vector>

ConfigurationMenu *configMenu;

int main(int argc, char **argv)
{
    if ( SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0 ) {
        cout << "Unable to init SDL: " << SDL_GetError() << endl;
        exit(1);
    }

    SDL_EnableUNICODE(1);
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

    //renderer   = new OpenGLRenderer(800, 600);
    Renderer *renderer  = new SoftwareRenderer(800, 600, false);
    renderer->setCurrent(renderer);

    Image *img          = new Image("testimage.png");

    Gui *gui            = new Gui(renderer->getScreen(), renderer->isSoftwareRenderer());
    gui->setCurrent(gui);

    configMenu          = new ConfigurationMenu();
    configMenu->setCurrent(configMenu);

    bool loop  = true;

    while(loop)
    {
        SDL_Event event;

        while(SDL_PollEvent(&event)) {

            switch (event.type) {
                case SDL_QUIT:
                    loop = false;
                    break;
                case SDL_USEREVENT:
                    cout << "User event" << endl;
                    if(event.user.code == 0) {
                        int *ints = (int*)event.user.data1;
                        delete renderer;
                        renderer = new SoftwareRenderer(800, 600, ints[0]);
                        renderer->setCurrent(renderer);
                        gui->setTarget(renderer->getScreen(), renderer->isSoftwareRenderer());
                        delete ints;
                    }
                    break;
            }

            gui->pushInput(event);
        }

        gui->logic();

        renderer->beginDraw();
        renderer->drawImage(img, 0, 0);
        gui->draw();
        renderer->swapBuffers();
    }

    delete configMenu;
    delete gui;
    delete img;
    delete renderer;

    return 0;
}
