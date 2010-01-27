#include "events.h"
#include "renderer.h"
#include "openglrenderer.h"
#include "softwarerenderer.h"
#include "image.h"
#include "gui.h"
#include "configurationmenu.h"
#include "loginmenu.h"
#include "networkClient.h"

#include <vector>

ConfigurationMenu *configMenu = 0;
LoginMenu* loginMenu = 0;

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

    loginMenu = new LoginMenu(200, 200);
    loginMenu->setCurrent(loginMenu);

	NetworkClient::setCurrent(new NetworkClient());

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
                    if ( event.user.code == EVENT_OPTIONS_OK ) {
                        int *ints = (int*)event.user.data1;

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

                        loginMenu = new LoginMenu(configMenu->getX(), configMenu->getY());
                        loginMenu->setCurrent(loginMenu);
                        delete configMenu;
                        configMenu = 0;

                        delete ints;
                    }
                    if ( event.user.code == EVENT_OPTIONS_CANCEL ) {
                        loginMenu = new LoginMenu(configMenu->getX(), configMenu->getY());
                        loginMenu->setCurrent(loginMenu);
                        delete configMenu;
                        configMenu = 0;
                    }
                    if ( event.user.code == EVENT_LOGIN_OK ) {
                        LoginMenu::LoginData* data = (LoginMenu::LoginData*)event.user.data1;
                        NetworkClient::current().disconnect();
	                    if ( !NetworkClient::current().connect(data->host, data->port, data->username,
                                                               data->password, data->createAccount) ) {
                            std::cout << "Could not access network "
                                      << data->host << ":" << data->port << "." << std::endl;
	                    }
                        delete data;
                    }
                    if ( event.user.code == EVENT_LOGIN_OPTIONS ) {
                        configMenu = new ConfigurationMenu(loginMenu->getX(), loginMenu->getY());
                        configMenu->setCurrent(configMenu);
                        delete loginMenu;
                        loginMenu = 0;
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

    if ( configMenu ) {
        delete configMenu;
        configMenu = 0;
    }
    if ( loginMenu ) {
        delete loginMenu;
        loginMenu = 0;
    }

    delete gui;
    delete img;
    delete renderer;
	delete &NetworkClient::current();

    return 0;
}
