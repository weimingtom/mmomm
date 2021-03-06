#include "animation.h"
#include "animationManager.h"
#include "chatWindow.h"
#include "clientActor.h"
#include "clientSprites.h"
#include "clientWorldInstance.h"
#include "configurationMenu.h"
#include "events.h"
#include "frameTimer.h"
#include "gui.h"
#include "inventory.h"
#include "image.h"
#include "imageManager.h"
#include "item.h"
#include "itemWindow.h"
#include "loginMenu.h"
#include "mouseSelectionMenu.h"
#include "networkClient.h"
#include "openglRenderer.h"
#include "random.h"
#include "renderer.h"
#include "softwareRenderer.h"

#include <SDL/SDL_ttf.h>
#include <vector>
#include <stdexcept>

ConfigurationMenu *configMenu = 0;
LoginMenu* loginMenu = 0;
ChatWindow* chatWindow = 0;
ItemWindow* itemWindow = 0;

int main(int argc, char **argv)
{
	Random::setCurrent(new Random());

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

    Inventory inventory;
    inventory.addItem(Item(0, "Sword",  100, ImageManager::current().getImage("data/icons/we_sword001.png")));
    inventory.addItem(Item(1, "Axe",    100, ImageManager::current().getImage("data/icons/we_axe001.png")));
    inventory.addItem(Item(2, "Mat",    100, ImageManager::current().getImage("data/icons/mat_001.png")));
    inventory.addItem(Item(3, "Item",   100, ImageManager::current().getImage("data/icons/item_001.png")));
    inventory.addItem(Item(4, "Helm",   100, ImageManager::current().getImage("data/icons/helm_001.png")));
    inventory.addItem(Item(5, "Armor",  100, ImageManager::current().getImage("data/icons/armor_001.png")));

    itemWindow = new ItemWindow(0, 0, inventory);

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
                        try {
                            if ( !NetworkClient::current().connect(data->host, data->port, data->username,
                                                                   data->password, data->createAccount) ) {
                                std::stringstream ss;
                                ss << "Could not access network "
                                   << data->host << ":" << data->port << "." << std::endl;
                                chatWindow->addText( ss.str(), ChatMessagePacket::CHAT_MESSAGE_SERVER );
                            }
                            else {
                                delete loginMenu;
                                loginMenu = 0;
                            }
                        }
                        catch(InvalidPacketException& e) {
                            cout << "more packet error?: " << e.what() << endl;
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
                    if ( event.user.code == EVENT_DELETE_WIDGET ) {
                        gcn::Widget* widget = (gcn::Widget*)event.user.data1;
                        delete widget;
                    }
                    break;
            }

            Gui::current().pushInput(event);
        }

		// Check packets
		while (NetworkClient::current().isActive()) {
			try {
				// Check packets until we run out
				std::auto_ptr<NetworkPacket> packet = NetworkClient::current().receive();

				if (!packet.get()) break;

				packet->respondClient();
			}
			catch (std::runtime_error& e) {
				cout << "Packet error: " << e.what() << endl;
			}
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
