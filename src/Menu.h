#pragma once
#include <SDL.h>

#include "Player.h"
#include "Window.h"
#include "Keyboard.h"
#include "AssetsManager.h"

class Menu
{
private:

	bool inGame;

	GameMode currentMode;

	Uint32 lastModeChange, msgStart;

	Player* player1, *player2;

	std::string message;

	//[ Private methods ]//

	void renderBg();

	void renderMsg();

public:

	Menu();

	bool uptate();

	void render();
};

