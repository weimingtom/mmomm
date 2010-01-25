#include "renderer.h"
#include "openglrenderer.h"
#include "softwarerenderer.h"
#include "image.h"
#include "gui.h"
#include "configurationmenu.h"
#include "networkClient.h"

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

	NetworkClient::setCurrent(new NetworkClient());

	//if (!NetworkClient::current().connect("localhost", 33033, "hangar", "awesome", false)) {
	//	std::cout << "Could not access network." << std::endl;
	//}

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
                    if(event.user.code == 0) {
                        int *ints = (int*)event.user.data1;

                        delete configMenu;
                        delete gui;
                        delete img;
                        delete renderer;

                        if(ints[3] != 0)
                            renderer    = new SoftwareRenderer(ints[0], ints[1], (ints[2] != 0));
                        else
                            renderer    = new OpenGLRenderer  (ints[0], ints[1], (ints[2] != 0));
                        renderer->setCurrent(renderer);

                        img             = new Image("testimage.png");

                        //gui->setTarget(renderer->getScreen(), renderer->isSoftwareRenderer());
                        gui             = new Gui(renderer->getScreen(), renderer->isSoftwareRenderer());
                        gui->setCurrent(gui);

                        configMenu      = new ConfigurationMenu();
                        configMenu->setCurrent(configMenu);

                        delete ints;
                    }
                    break;
            }

            gui->pushInput(event);
        }

		// Check packets
		while (NetworkClient::current().isActive()) {
			// Check packets until we run out
			std::auto_ptr<NetworkPacket> packet = NetworkClient::current().receive();
			if (!packet.get()) break;

			//packet->respondClient();
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
	delete &NetworkClient::current();

    return 0;
}
