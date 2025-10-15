#pragma once
#include <SDL.h>

#include "Player.h"
#include "Window.h"
#include "Keyboard.h"

class Menu
{
private:

	SDL_Texture* bg;

	Player* player1, *player2;

public:

	Menu();

	bool uptate();

	void render();
};

