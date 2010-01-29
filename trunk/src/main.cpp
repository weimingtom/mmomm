#include "events.h"
#include "renderer.h"
#include "openglRenderer.h"
#include "softwareRenderer.h"
#include "image.h"
#include "imageManager.h"
#include "animation.h"
#include "animationManager.h"
#include "gui.h"
#include "configurationMenu.h"
#include "loginMenu.h"
#include "networkClient.h"
#include "chatWindow.h"

#include <vector>

ConfigurationMenu *configMenu = 0;
LoginMenu* loginMenu = 0;
ChatWindow* chatWindow = 0;

int main(int argc, char **argv)
{
    if ( SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0 ) {
        cout << "Unable to init SDL: " << SDL_GetError() << endl;
        exit(1);
    }

    SDL_EnableUNICODE(1);
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

    Renderer *renderer  = new SoftwareRenderer(640, 480, false);
    renderer->setCurrent(renderer);

    ImageManager::setCurrent(new ImageManager());

    ImageManager::shared_ptr img  = ImageManager::current().getImage("testimage.png");
#ifndef NDEBUG
    ImageManager::current().use_count();
#endif

    AnimationManager::setCurrent(new AnimationManager());

    Gui::setCurrent(new Gui(renderer->getScreen(), renderer->isSoftwareRenderer()));

    loginMenu = new LoginMenu(100, 150);
    loginMenu->setCurrent(loginMenu);

    chatWindow = new ChatWindow(300, 150);
    chatWindow->setCurrent(chatWindow);

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

                        if((ints[3] != 0) != renderer->isSoftwareRenderer())
                        {
                            delete &Gui::current();
                            delete renderer;

                            if(ints[3] != 0)
                                renderer    = new SoftwareRenderer(ints[0], ints[1], (ints[2] != 0));
                            else
                                renderer    = new OpenGLRenderer  (ints[0], ints[1], (ints[2] != 0));
                            renderer->setCurrent(renderer);

                            ImageManager::current().reloadImages();

                            Gui::setCurrent(new Gui(renderer->getScreen(), renderer->isSoftwareRenderer()));
                        }

                        loginMenu = new LoginMenu(configMenu->getX(), configMenu->getY());
                        loginMenu->setCurrent(loginMenu);
                        delete &ConfigurationMenu::current();
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

            Gui::current().pushInput(event);
        }

		// Check packets
		while (NetworkClient::current().isActive()) {
			// Check packets until we run out
			std::auto_ptr<NetworkPacket> packet = NetworkClient::current().receive();
			if (!packet.get()) break;

			//packet->respondClient();
		}

        Gui::current().logic();
        //AnimationManager::current().update(time);

        renderer->beginDraw();
        renderer->drawImage(img.get(), 0, 0);
        Gui::current().draw();
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
    if ( chatWindow ) {
        delete chatWindow;
        chatWindow = 0;
    }

    delete &Gui::current();
    delete &ImageManager::current();
    delete renderer;
	delete &NetworkClient::current();

    return 0;
}
