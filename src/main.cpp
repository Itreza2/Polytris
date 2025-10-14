#define SDL_MAIN_HANDLED

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Window.h"
#include "AssetsManager.h"
#include "Keyboard.h"
#include "Menu.h"

int main(unsigned int argc, char** argv) 
{
	//Initialize the window and load the assets
	AssetsManager::getLib()->loadGroup("rsc\\assets.load", Window::getWindow()->renderer);

	Menu menu = Menu();

	srand(static_cast<unsigned int>(time(NULL)));
	Uint32 lastFrame = SDL_GetTicks();

	bool  exit = false;
	while (!exit) {
		exit = (Keyboard::getKeyboard()->catchEvents()
			 || menu.uptate());

		if (SDL_GetTicks() - lastFrame > 16) {

			SDL_RenderClear(Window::getWindow()->renderer);
			menu.render();
			SDL_RenderPresent(Window::getWindow()->renderer);

			lastFrame = SDL_GetTicks();
		}
	}
	return EXIT_SUCCESS;
}