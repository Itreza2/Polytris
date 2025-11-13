#include "Leaderboard.h"

Highscore::Highscore(std::string text)
{
	std::string word;
	std::stringstream stream(text);

	std::getline(stream, word, ',');
	author = word;
	std::getline(stream, word, ',');
	date = word;
	std::getline(stream, word, ',');
	score = stoi(word);
	std::getline(stream, word, ',');
	time = stoi(word);
}

std::string Highscore::serialize()
{
	return (
		author + "," + date + ","
		+ std::to_string(score) + "," + std::to_string(time)
	);
}

void Leaderboard::load()
{
	std::string line;
	std::vector<std::string> row;
	char* systemDrive;
	size_t uselessBuffer;

	leaderboard = {};
	leaderboardWeekly = {};

	_dupenv_s(&systemDrive, &uselessBuffer, "SystemDrive");

	std::ifstream file( // All-time leaderboard
		std::string(systemDrive) + "\\ProgramData\\Polytris\\highscores\\" + std::to_string(mode)
	);
	if (file.is_open()) {
		while (std::getline(file, line))
			leaderboard.push_back(Highscore(line));
	} file.close();
	file = std::ifstream( // Weekly leaderboard
		std::string(systemDrive) + "\\ProgramData\\Polytris\\highscores\\" + std::to_string(mode) + "w"
	);
	if (file.is_open()) {
		while (std::getline(file, line))
			leaderboardWeekly.push_back(Highscore(line));
	} file.close();
	free(systemDrive);
}

void Leaderboard::save()
{
	char* systemDrive;
	size_t uselessBuffer;

	_dupenv_s(&systemDrive, &uselessBuffer, "SystemDrive");

	std::ofstream file(
		std::string(systemDrive) + "\\ProgramData\\Polytris\\highscores\\" + std::to_string(mode),
		std::ofstream::trunc
	);
	if (!file.is_open()) {
		std::filesystem::create_directory(std::string(systemDrive) + "\\ProgramData\\Polytris");
		std::filesystem::create_directory(std::string(systemDrive) + "\\ProgramData\\Polytris\\highscores");
	}
	file = std::ofstream(
		std::string(systemDrive) + "\\ProgramData\\Polytris\\highscores\\" + std::to_string(mode),
		std::ofstream::trunc
	);
	for (Highscore line : leaderboard) {
		file << line.serialize() << std::endl;
	}
	file = std::ofstream(
		std::string(systemDrive) + "\\ProgramData\\Polytris\\highscores\\" + std::to_string(mode) + "w",
		std::ofstream::trunc
	);
	for (Highscore line : leaderboardWeekly) {
		file << line.serialize() << std::endl;
	}
}

void Leaderboard::render()
{

}

bool Leaderboard::scoreGreaterThan(Highscore score1, Highscore score2) const
{
	if (mode == GM_MARATHON and score1.score >= score2.score)
		return true;
	if (mode == GM_SPRINT40L and score1.time <= score2.time)
		return true;
	return false;
}

Leaderboard::Leaderboard(GameMode mode)
{
	this->mode = mode;
	toRender = true;

	load();
}

void Leaderboard::submit(Highscore newScore)
{	
	// All-time leaderboard
	int pos = leaderboard.size();
	for (int i = leaderboard.size() - 1; i >= 0; i--) {
		if (scoreGreaterThan(newScore, leaderboard[i]))
			pos = i;
	}
	leaderboard.insert(leaderboard.begin() + pos, newScore);
	if (leaderboard.size() > 10)
		leaderboard.pop_back();

	// Weekly leaderboard
	pos = leaderboardWeekly.size();
	for (int i = leaderboardWeekly.size() - 1; i >= 0; i--) {
		if (scoreGreaterThan(newScore, leaderboardWeekly[i]))
			pos = i;
	}
	leaderboardWeekly.insert(leaderboardWeekly.begin() + pos, newScore);
	if (leaderboardWeekly.size() > 10)
		leaderboardWeekly.pop_back();
	
	save();
	toRender = true;
}

SDL_Texture* Leaderboard::getTexture()
{
	if (toRender)
		render();
	return texture;
}