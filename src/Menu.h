#pragma once
#include <SDL.h>

#include "Player.h"
#include "Window.h"
#include "Keyboard.h"
#include "AssetsManager.h"

class Menu
{
private:

	GameMode currentMode;

	Player* player1, *player2;

	//[ Private methods ]//

	void renderBg();

public:

	Menu();

	bool uptate();

	void render();
};

