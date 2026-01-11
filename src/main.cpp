#define SDL_MAIN_HANDLED

#include <SDL.h>

#include "Window.h"
#include "AssetsManager.h"
#include "Keyboard.h"
#include "Menu.h"

int main(unsigned int argc, char** argv) 
{
	//Initialize the window and load the assets
	AssetsManager::getLib()->loadGroup("rsc\\assets.load", Window::getWindow()->renderer);

	Menu* menu = new Menu();

	srand(static_cast<unsigned int>(time(NULL)));
	Uint32 lastFrame = SDL_GetTicks();

	bool  exit = false;
	while (!exit) {
		exit = (Keyboard::getKeyboard()->catchEvents()
			 || menu->update());

		if (SDL_GetTicks() - lastFrame > 16) {

			SDL_RenderClear(Window::getWindow()->renderer);
			menu->render();
			SDL_RenderPresent(Window::getWindow()->renderer);

			lastFrame = SDL_GetTicks();
		}
	}
	delete menu;
	return EXIT_SUCCESS;
}