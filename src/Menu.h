#pragma once
#include <SDL.h>

#include "Player.h"
#include "Leaderboard.h"
#include "Window.h"
#include "Keyboard.h"
#include "AttackBuffer.h"
#include "AssetsManager.h"

class Menu
{
private:

	bool inGame;

	GameMode currentMode;

	Uint32 lastModeChange, msgStart;

	Leaderboard* marathonLeaderboard, *sprintLeaderboard;

	Player* player1, *player2;

	AttackBuffer* attackBuffer;

	std::string message;

	//[ Private methods ]//

	void renderBg();

	void renderMsg();

	void newAttackBuffer();

public:

	Menu();

	bool update();

	void render();
};