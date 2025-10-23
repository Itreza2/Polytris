#include "Menu.h"

void Menu::renderBg()
{
	SDL_Rect dst;
	int tick = (int)(SDL_GetTicks() * 0.05) % 768;

	//Background animation
	SDL_RenderCopy(Window::getWindow()->renderer, AssetsManager::getLib()->getTexture("background"), NULL, NULL);
	for (int i = -1600; i < 800; i += 768) {
		dst = { 768, -i - tick, 1920, 1600 };
		SDL_RenderCopy(Window::getWindow()->renderer, AssetsManager::getLib()->getTexture("redBand"), NULL, &dst);
		dst = { 0, -i - tick, 1920, 1600 };
		SDL_RenderCopy(Window::getWindow()->renderer, AssetsManager::getLib()->getTexture("redBand"), NULL, &dst);
		dst = { -768, -i - tick, 1920, 1600 };
		SDL_RenderCopy(Window::getWindow()->renderer, AssetsManager::getLib()->getTexture("redBand"), NULL, &dst);
	}
	for (int i = -1600; i < 800; i += 768) {
		dst = { 768, i + tick, 1920, 1600 };
		SDL_RenderCopy(Window::getWindow()->renderer, AssetsManager::getLib()->getTexture("blueBand"), NULL, &dst);
		dst = { 0, i + tick, 1920, 1600 };
		SDL_RenderCopy(Window::getWindow()->renderer, AssetsManager::getLib()->getTexture("blueBand"), NULL, &dst);
		dst = { -768, i + tick, 1920, 1600 };
		SDL_RenderCopy(Window::getWindow()->renderer, AssetsManager::getLib()->getTexture("blueBand"), NULL, &dst);
	}
	SDL_RenderCopy(Window::getWindow()->renderer, AssetsManager::getLib()->getTexture("alpha50"), NULL, NULL);
}

Menu::Menu() 
{ 
	currentMode = GM_MARATHON;

	player1 = new Player(PLAYER_1, currentMode, PS_IDLE); 
	player2 = new Player(PLAYER_2, currentMode, PS_IDLE);
}

bool Menu::uptate() 
{
	Keyboard* keyboard = Keyboard::getKeyboard();

	if (keyboard->keyDown(KEY_EXIT)) {
		return true;
	}
	if (player1->update()) {
		free(player1);
		player1 = new Player(PLAYER_1, currentMode);
	}
	if (player2->update()) {
		free(player2);
		player2 = new Player(PLAYER_2, currentMode);
	}

	return false;
}

//To refactor : quick implementation for test purposes
void Menu::render()
{
	SDL_Rect p1Area = { 192, 212, 576, 656 };
	SDL_Rect p2Area = { 1152, 212, 576, 656 };

	renderBg();

	SDL_RenderCopy(Window::getWindow()->renderer, player1->render(), NULL, &p1Area);
	SDL_RenderCopy(Window::getWindow()->renderer, player2->render(), NULL, &p2Area);
}