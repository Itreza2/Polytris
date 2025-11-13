#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <filesystem>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

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
};