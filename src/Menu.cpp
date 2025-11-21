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
	if (inGame)
		dst = { 0, 0, 1920, 1080 };
	else {
		dst = { 84, 0, 720, 1080 };
		SDL_RenderCopy(Window::getWindow()->renderer, AssetsManager::getLib()->getTexture("alpha25"), NULL, NULL);
	}
	SDL_RenderCopy(Window::getWindow()->renderer, AssetsManager::getLib()->getTexture("alpha50"), NULL, &dst);
}

void Menu::renderMsg()
{
	int pos = (SDL_GetTicks() - msgStart) / 20;

	int lenght = createText(Window::getWindow()->renderer, -pos, 1012, message.c_str(),
		AssetsManager::getLib()->getFont("futuraM"), A_W);
	createText(Window::getWindow()->renderer, lenght - pos, 1012, message.c_str(),
		AssetsManager::getLib()->getFont("futuraM"), A_W);

	if (pos >= lenght)
		msgStart = SDL_GetTicks();
}

void Menu::newAttackBuffer()
{
	delete attackBuffer;
	delete player1;
	delete player2;

	attackBuffer = new AttackBuffer();
	player1 = new Player(PLAYER_1, currentMode, attackBuffer, PS_IDLE);
	player2 = new Player(PLAYER_2, currentMode, attackBuffer, PS_IDLE);
}

void Menu::submitScore(Highscore* newScore)
{
	if (newScore != nullptr) {
		if (currentMode == GM_MARATHON)
			marathonLeaderboard->submit(*newScore);
		if (currentMode == GM_SPRINT40L)
			sprintLeaderboard->submit(*newScore);
	}
}

Menu::Menu() 
{ 
	inGame = false;
	currentMode = GM_MARATHON;
	lastModeChange = 0; msgStart = SDL_GetTicks();

	marathonLeaderboard = new Leaderboard(GM_MARATHON);
	sprintLeaderboard = new Leaderboard(GM_SPRINT40L);

	player1 = nullptr; 
	player2 = nullptr;
	attackBuffer = new AttackBuffer();

	std::ifstream file;
	file.open("rsc\\mainMenuMessage.txt");
	std::getline(file, message);
}

bool Menu::update() 
{
	Keyboard* keyboard = Keyboard::getKeyboard();

	if (inGame) {
		if (keyboard->keyDown(KEY_EXIT)) {
			submitScore(player1->getFinalScore());
			submitScore(player2->getFinalScore());
			inGame = false; //Back to main menu
			delete player1;
			delete player2;
			lastModeChange = SDL_GetTicks();
		}
		if (player1->update()) {
			if (attackBuffer->isOver())
				newAttackBuffer();
			else {
				submitScore(player1->getFinalScore());
				delete player1;
				player1 = new Player(PLAYER_1, currentMode, attackBuffer);
			}
		}
		if (player2->update()) {
			if (attackBuffer->isOver())
				newAttackBuffer();
			else {
				submitScore(player2->getFinalScore());
				delete player2;
				player2 = new Player(PLAYER_2, currentMode, attackBuffer);
			}
		}
	}
	else { //Main menu
		if (keyboard->keyDown(KEY_EXIT) && SDL_GetTicks() - lastModeChange > 300) {
			return true; //Exit the application
		}
		if (keyboard->keyDown(KEY_UP) && !keyboard->keyDown(KEY_DOWN) && SDL_GetTicks() - lastModeChange > 200) {
			if (currentMode)
				currentMode = static_cast<GameMode>(static_cast<std::underlying_type<GameMode>::type>(currentMode) - 1);
			lastModeChange = SDL_GetTicks();
		}
		if (keyboard->keyDown(KEY_DOWN) && !keyboard->keyDown(KEY_UP) && SDL_GetTicks() - lastModeChange > 200) {
			if (currentMode != 2)
				currentMode = static_cast<GameMode>(static_cast<std::underlying_type<GameMode>::type>(currentMode) + 1);
			lastModeChange = SDL_GetTicks();
		}
		if (keyboard->keyDown(KEY_ANY)) {
			inGame = true;
			delete attackBuffer;
			attackBuffer = new AttackBuffer();
			player1 = new Player(PLAYER_1, currentMode, attackBuffer, PS_IDLE);
			player2 = new Player(PLAYER_2, currentMode, attackBuffer, PS_IDLE);
		}
	}
	return false;
}

//To refactor : quick implementation for test purposes
void Menu::render()
{
	SDL_Rect p1Area = { 256, 212, 576, 656 };
	SDL_Rect p2Area = { 1088, 212, 576, 656 };
	SDL_Rect dst;

	renderBg();

	if (inGame) {
		SDL_RenderCopy(Window::getWindow()->renderer, player1->render(), NULL, &p1Area);
		SDL_RenderCopy(Window::getWindow()->renderer, player2->render(), NULL, &p2Area);
	}
	else {
		dst = { 110, 60, 250, 250 };
		SDL_RenderCopy(Window::getWindow()->renderer, AssetsManager::getLib()->getTexture("logo"), NULL, &dst);
		createText(Window::getWindow()->renderer, 310, 285, "olytris",
			AssetsManager::getLib()->getFont("futuraCountdown"), A_SW);
		createText(Window::getWindow()->renderer, 620, 285, "v1.0.1",
			AssetsManager::getLib()->getFont("futuraS"), A_SW);

		dst = { 0, 980, 1920, 64 };
		SDL_RenderCopy(Window::getWindow()->renderer, AssetsManager::getLib()->getTexture("alphaRed"), NULL, &dst);
		renderMsg();

		dst = { 1060, 120, 600, 820 };
		switch (currentMode)
		{
		case GM_MARATHON:
			createText(Window::getWindow()->renderer, 200, 470, "Marathon",
				AssetsManager::getLib()->getFont("futuraH"), A_W);
			createText(Window::getWindow()->renderer, 200, 590, "Sprint (40L)",
				AssetsManager::getLib()->getFont("futuraB"), A_W);
			createText(Window::getWindow()->renderer, 200, 710, "Versus 1c1",
				AssetsManager::getLib()->getFont("futuraB"), A_W);
			SDL_RenderCopy(Window::getWindow()->renderer, marathonLeaderboard->getTexture(), NULL, &dst);
			break;
		case GM_SPRINT40L:
			createText(Window::getWindow()->renderer, 200, 470, "Marathon",
				AssetsManager::getLib()->getFont("futuraB"), A_W);
			createText(Window::getWindow()->renderer, 200, 590, "Sprint (40L)",
				AssetsManager::getLib()->getFont("futuraH"), A_W);
			createText(Window::getWindow()->renderer, 200, 710, "Versus 1c1",
				AssetsManager::getLib()->getFont("futuraB"), A_W);
			SDL_RenderCopy(Window::getWindow()->renderer, sprintLeaderboard->getTexture(), NULL, &dst);
			break;
		case GM_VERSUS:
			createText(Window::getWindow()->renderer, 200, 470, "Marathon",
				AssetsManager::getLib()->getFont("futuraB"), A_W);
			createText(Window::getWindow()->renderer, 200, 590, "Sprint (40L)",
				AssetsManager::getLib()->getFont("futuraB"), A_W);
			createText(Window::getWindow()->renderer, 200, 710, "Versus 1c1",
				AssetsManager::getLib()->getFont("futuraH"), A_W);
			createText(Window::getWindow()->renderer, 1360, 530, "Pas de classement pour ce mode de jeu :(",
				AssetsManager::getLib()->getFont("futuraM"), A_CENTER);
			break;
		}
	}
}