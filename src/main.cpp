#include "events.h"
#include "renderer.h"
#include "openglRenderer.h"
#include "softwareRenderer.h"
#include "image.h"
#include "imageManager.h"
#include "animation.h"
#include "animationManager.h"
#include "clientSprites.h"
#include "gui.h"
#include "configurationMenu.h"
#include "loginMenu.h"
#include "mouseSelectionMenu.h"
#include "networkClient.h"
#include "chatWindow.h"
#include "clientWorldInstance.h"
#include "clientActor.h"
#include "frameTimer.h"

#include <vector>
#include <SDL/SDL_ttf.h>

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

    if ( TTF_Init() < 0 ) {
        cout << "Unable to init TTF: " << TTF_GetError() << endl;
        exit(1);
    }

    Renderer *renderer = new SoftwareRenderer(Vector2D(640, 480), false);
    renderer->setCurrent(renderer);

    ImageManager::setCurrent(new ImageManager());
#ifndef NDEBUG
    ImageManager::current().use_count();
#endif

    AnimationManager::setCurrent(new AnimationManager());
    ClientSprites::Setup();

    WorldInstance::setCurrent(new ClientWorldInstance());

    Gui::setCurrent(new Gui(renderer->getScreen(), renderer->isSoftwareRenderer()));

    loginMenu = new LoginMenu(100, 150);
    loginMenu->setCurrent(loginMenu);

    chatWindow = new ChatWindow(300, 150);
    chatWindow->setCurrent(chatWindow);

	NetworkClient::setCurrent(new NetworkClient());

	FrameTimer::setCurrent(new FrameTimer());

    bool loop  = true;

    while( loop )
    {
        SDL_Event event;

        while( SDL_PollEvent(&event) ) {

            switch (event.type) {
                case SDL_KEYDOWN:
                    ClientWorldInstance::current().keyDown(event.key.keysym.sym);
                    break;
                case SDL_KEYUP:
                    ClientWorldInstance::current().keyUp(event.key.keysym.sym);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    ClientWorldInstance::current().mouseDown(event.button.button);
                    break;
                case SDL_MOUSEBUTTONUP:
                    ClientWorldInstance::current().mouseUp(event.button.button);
                    break;
                case SDL_MOUSEMOTION:
                    ClientWorldInstance::current().mouseMotion(Vector2D(event.motion.x, event.motion.y));
                    break;
                case SDL_QUIT:
                    loop = false;
                    break;
                case SDL_USEREVENT:
                    if ( event.user.code == EVENT_OPTIONS_OK ) {
                        int *ints = (int*)event.user.data1;

                        delete &ChatWindow::current();
                        delete &Gui::current();
                        delete renderer;

                        if(ints[3] != 0)
                            renderer    = new SoftwareRenderer(Vector2D(ints[0], ints[1]), (ints[2] != 0));
                        else
                            renderer    = new OpenGLRenderer  (Vector2D(ints[0], ints[1]), (ints[2] != 0));
                        renderer->setCurrent(renderer);

                        ImageManager::current().reloadImages();

                        Gui::setCurrent(new Gui(renderer->getScreen(), renderer->isSoftwareRenderer()));

                        if( loginMenu != NULL ) {
                            delete loginMenu;
                            loginMenu = 0;
                        }

                        loginMenu = new LoginMenu(configMenu->getX(), configMenu->getY());
                        loginMenu->setCurrent(loginMenu);
                        delete &ConfigurationMenu::current();
                        chatWindow = new ChatWindow(300, 150);
                        chatWindow->setCurrent(chatWindow);
                        configMenu = 0;

                        delete ints;
                    }
                    if ( event.user.code == EVENT_OPTIONS_CANCEL ) {
                        if( loginMenu != NULL ) {
                            delete loginMenu;
                            loginMenu = 0;
                        }

                        loginMenu = new LoginMenu(configMenu->getX(), configMenu->getY());
                        loginMenu->setCurrent(loginMenu);
                        delete configMenu;
                        configMenu = 0;
                    }
                    if ( event.user.code == EVENT_LOGIN_OK ) {
                        LoginMenu::LoginData* data = (LoginMenu::LoginData*)event.user.data1;
                        NetworkClient::current().disconnect();
                        delete &WorldInstance::current();
                        WorldInstance::setCurrent(new ClientWorldInstance());
	                    if ( !NetworkClient::current().connect(data->host, data->port, data->username,
                                                               data->password, data->createAccount) ) {
                            std::stringstream ss;
                            ss << "Could not access network "
                               << data->host << ":" << data->port << "." << std::endl;
                            chatWindow->addText( ss.str(), ChatMessagePacket::CHAT_MESSAGE_SERVER );
	                    }
                        delete data;
                    }
                    if ( event.user.code == EVENT_LOGIN_OPTIONS ) {
                        if( configMenu != NULL ) {
                            delete configMenu;
                            configMenu = 0;
                        }

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

			packet->respondClient();
		}

        Gui::current().logic();
		AnimationManager::current().update(FrameTimer::current().elapsed());
        WorldInstance::current().update(FrameTimer::current().elapsed());

        renderer->beginDraw();
        ClientWorldInstance::current().render();
        Gui::current().draw();
        renderer->swapBuffers();

		FrameTimer::current().step();
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

    ClientSprites::Delete();
    delete &WorldInstance::current();
    delete &Gui::current();
    delete &AnimationManager::current();
    delete &ImageManager::current();
    delete renderer;
	delete &NetworkClient::current();
	delete &FrameTimer::current();

    return 0;
}
