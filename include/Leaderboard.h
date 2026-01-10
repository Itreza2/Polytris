#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <filesystem>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iostream>

#include "Window.h"
#include "AssetsManager.h"
#include "textTools.cpp"

enum GameMode {
	GM_MARATHON = 0,
	GM_SPRINT40L = 1,
	GM_VERSUS = 2,
};

struct Highscore {
	std::string author;
	std::string date;
	int score;
	int time;

	Highscore(std::string text);

	Highscore(std::string author, int score, int time);

	std::string serialize();
};

class Leaderboard
{
private:

	GameMode mode;

	std::vector<Highscore> leaderboard;
	std::vector<Highscore> leaderboardWeekly;

	bool toRender;
	SDL_Texture* texture;

	//[ Private Methods ]//

	void load();

	void save();

	void render();

	bool scoreGreaterThan(Highscore score1, Highscore score2) const;

public:

	Leaderboard(GameMode mode);

	void submit(Highscore newScore);

	SDL_Texture* getTexture();

	static std::string chronoText(Uint32 timer);

	static void saveSessionDate();
};