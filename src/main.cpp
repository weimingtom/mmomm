#include "renderer.h"
#include "openglrenderer.h"
#include "softwarerenderer.h"
#include "image.h"
#include "gui.h"

gcn::Label *label;
gcn::Window *window;

int main(int argc, char **argv)
{
    if ( SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0 ) {
        cout << "Unable to init SDL: " << SDL_GetError() << endl;
        exit(1);
    }

    SDL_EnableUNICODE(1);
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

    //renderer   = new OpenGLRenderer(800, 600);
    renderer   = new SoftwareRenderer(800, 600);
    Image *img = new Image("testimage.png");
    Gui *gui   = new Gui(renderer->getScreen(), renderer->isSoftwareRenderer());
    label = new gcn::Label("Hello World");
    label->setPosition(280, 220);
    gui->addWidget(label);
    window = new gcn::Window("Test window OF DOOM!");
    window->setPosition(50, 50);
    window->setSize(200, 200);
    window->setVisible(true);
    gui->addWidget(window);

    bool loop  = true;

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

        renderer->beginDraw();
        renderer->drawImage(img, 0, 0);
        gui->draw();
        renderer->swapBuffers();
    }

    gui->removeWidget(label);
    gui->removeWidget(window);

    delete gui;
    delete img;
    delete renderer;
    delete label;
    delete window;

    return 0;
}
