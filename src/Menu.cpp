#include "Menu.h"

Menu::Menu() 
{ 
	bg = SDL_CreateTexture(Window::getWindow()->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 960, 540);

	player1 = new Player(PLAYER_1); 
}

bool Menu::uptate() 
{
	Keyboard* keyboard = Keyboard::getKeyboard();

	if (keyboard->keyDown(KEY_EXIT)) {
		return true;
	}
	if (player1->update()) {
		free(player1);
		player1 = new Player(PLAYER_1);
	}

	return false;
}

//To refactor : quick implementation for test purposes
void Menu::render()
{
	SDL_Rect p1Area = { 0, 0, 10 * 16, 20 * 16 };

	SDL_RenderCopy(Window::getWindow()->renderer, player1->render(), NULL, &p1Area);
}