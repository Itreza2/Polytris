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

Highscore::Highscore(std::string author, int score, int time)
{
	this->author = author;
	this->score = score;
	this->time = time;

	time_t timestamp = std::time(NULL);
	struct tm datetime;

#ifdef _WIN32
	localtime_s(&datetime, &timestamp);
#else
	localtime_r(&timestamp, &datetime);
#endif
	
	char output[50];
	strftime(output, 50, "%b %e %H:%M:%S %Y", &datetime);
	date = output;
}

std::string Highscore::serialize()
{
	return (
		author + "," + date + ","
		+ std::to_string(score) + "," + std::to_string(time)
	);
}

std::string Leaderboard::getWeekString(const std::chrono::system_clock::time_point& tp)
{
	time_t timestamp = std::chrono::system_clock::to_time_t(tp);
	struct tm datetime;
	localtime_r(&timestamp, &datetime);
	char buffer[10];
	strftime(buffer, sizeof(buffer), "%W", &datetime);
	return std::string(buffer);
}

void Leaderboard::load()
{
	std::string line;
	std::vector<std::string> row;
	std::string configFolder;

	leaderboard = {};
	leaderboardWeekly = {};
	int lastSession = -1;

#ifdef _WIN32
	char* systemDrive;
	size_t uselessBuffer;
	_dupenv_s(&systemDrive, &uselessBuffer, "SystemDrive");
	configFolder = std::string(systemDrive) + "\\ProgramData\\Polytris";
#elif __linux__
	configFolder = std::string(getenv("HOME")) + "/.config/Polytris";
#endif

	std::chrono::system_clock::time_point currentDate = std::chrono::system_clock::now();

	std::ifstream file( // All-time leaderboard
		configFolder + "/highscores/" + std::to_string(mode)
	);
	if (file.is_open()) {
		while (std::getline(file, line))
			leaderboard.push_back(Highscore(line));
	} file.close();
	file = std::ifstream( // Week of last session
		configFolder + "/last_session_week"
	);
	if (file.is_open()) {
		std::getline(file, line);
		lastSession = atoi(line.c_str());
	}
	file = std::ifstream( // Weekly leaderboard
		configFolder + "/highscores/" + std::to_string(mode) + "w"
	);
	if (file.is_open() && lastSession == atoi(getWeekString(currentDate).c_str())) {
		while (std::getline(file, line))
			leaderboardWeekly.push_back(Highscore(line));
	} file.close();

#ifdef _WIN32
	free(systemDrive);
#endif
}

void Leaderboard::save()
{
	std::string configFolder;

#ifdef _WIN32
	char* systemDrive;
	size_t uselessBuffer;
	_dupenv_s(&systemDrive, &uselessBuffer, "SystemDrive");
	configFolder = std::string(systemDrive) + "\\ProgramData\\Polytris";
#elif __linux__
	configFolder = std::string(getenv("HOME")) + "/.config/Polytris";
#endif

	std::chrono::system_clock::time_point currentDate = std::chrono::system_clock::now();

	std::ofstream file(
		configFolder + "/highscores/" + std::to_string(mode),
		std::ofstream::trunc
	);
	if (!file.is_open()) {
		std::filesystem::create_directory(configFolder);
		std::filesystem::create_directory(configFolder + "/highscores");
	}
	file = std::ofstream(
		configFolder + "/highscores/" + std::to_string(mode),
		std::ofstream::trunc
	);
	for (Highscore line : leaderboard) {
		file << line.serialize() << std::endl;
	}
	file = std::ofstream(
		configFolder + "/highscores/" + std::to_string(mode) + "w",
		std::ofstream::trunc
	);
	for (Highscore line : leaderboardWeekly) {
		file << line.serialize() << std::endl;
	}

#ifdef _WIN32
	free(systemDrive);
#endif
}

void Leaderboard::render()
{
	SDL_SetRenderTarget(Window::getWindow()->renderer, texture);

	SDL_RenderCopy(Window::getWindow()->renderer, AssetsManager::getLib()->getTexture("alpha0"), NULL, NULL);
	SDL_RenderCopy(Window::getWindow()->renderer, AssetsManager::getLib()->getTexture("leaderboard"), NULL, NULL);

	if (mode == GM_MARATHON)
		createText(Window::getWindow()->renderer, 300, 30, "Top 10 Marathon",
			AssetsManager::getLib()->getFont("futuraM"), A_CENTER);
	if (mode == GM_SPRINT40L)
		createText(Window::getWindow()->renderer, 300, 30, "Top 10 Sprint 40L",
			AssetsManager::getLib()->getFont("futuraM"), A_CENTER);
	for (int i = 0; i < leaderboard.size(); i++) {
		createText(Window::getWindow()->renderer, 15, 78 + i * 33, ("#" + std::to_string(i + 1) + " : " + leaderboard[i].author).c_str(),
			AssetsManager::getLib()->getFont("futuraS"), A_W);
		createText(Window::getWindow()->renderer, 280, 78 + i * 33, leaderboard[i].date.c_str(),
			AssetsManager::getLib()->getFont("futuraS"), A_CENTER);
		if (mode == GM_MARATHON)
			createText(Window::getWindow()->renderer, 585, 78 + i * 33, (std::to_string(leaderboard[i].score) + "pt").c_str(),
				AssetsManager::getLib()->getFont("futuraS"), A_E);
		if (mode == GM_SPRINT40L)
			createText(Window::getWindow()->renderer, 585, 78 + i * 33, chronoText(leaderboard[i].time - 3000).c_str(),
				AssetsManager::getLib()->getFont("futuraS"), A_E);
	}
	createText(Window::getWindow()->renderer, 300, 420, "Top 10 Hebdomadaire",
		AssetsManager::getLib()->getFont("futuraM"), A_CENTER);
	for (int i = 0; i < leaderboardWeekly.size(); i++) {
		createText(Window::getWindow()->renderer, 15, 468 + i * 33, ("#" + std::to_string(i + 1) + " : " + leaderboardWeekly[i].author).c_str(),
			AssetsManager::getLib()->getFont("futuraS"), A_W);
		createText(Window::getWindow()->renderer, 280, 468 + i * 33, leaderboardWeekly[i].date.c_str(),
			AssetsManager::getLib()->getFont("futuraS"), A_CENTER);
		if (mode == GM_MARATHON)
			createText(Window::getWindow()->renderer, 585, 468 + i * 33, (std::to_string(leaderboardWeekly[i].score) + "pt").c_str(),
				AssetsManager::getLib()->getFont("futuraS"), A_E);
		if (mode == GM_SPRINT40L)
			createText(Window::getWindow()->renderer, 585, 468 + i * 33, chronoText(leaderboardWeekly[i].time - 3000).c_str(),
				AssetsManager::getLib()->getFont("futuraS"), A_E);
	}


	SDL_SetRenderTarget(Window::getWindow()->renderer, NULL);
}

std::string Leaderboard::chronoText(Uint32 timer)
{
	return (std::to_string(timer / 60000) + "'"
		+ std::to_string(timer / 1000 % 60) + "''");
}

void Leaderboard::saveSessionDate()
{
	std::string configFolder;

#ifdef _WIN32
	char* systemDrive;
	size_t uselessBuffer;
	_dupenv_s(&systemDrive, &uselessBuffer, "SystemDrive");
	configFolder = std::string(systemDrive) + "\\ProgramData\\Polytris";
#elif __linux__
	configFolder = std::string(getenv("HOME")) + "/.config/Polytris";
#endif

	std::chrono::system_clock::time_point currentDate = std::chrono::system_clock::now();

	std::ofstream file(
		configFolder + "/last_session_week",
		std::ofstream::trunc
	);
	file << getWeekString(currentDate);

#ifdef _WIN32
	free(systemDrive);
#endif
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

	texture = SDL_CreateTexture(Window::getWindow()->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 600, 820);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

	load();
	save(); // Ensure the great weekly purge
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