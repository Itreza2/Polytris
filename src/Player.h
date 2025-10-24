#pragma once
#include <exception>
#include <SDL.h>
#include <vector>
#include <algorithm>

#include "Block.h"
#include "Window.h"
#include "Keyboard.h"
#include "AssetsManager.h"

#include "textTools.cpp"

const Uint32 ARR = 100;
const Uint32 DAS = 200;
const Uint32 HD_Lock = 250;
const Uint32 ROT = 400;

enum PlayerStatus {
	PS_IDLE,
	PS_GAME,
	PS_LOSS,
	PS_WIN,
};

enum GameMode {
	GM_MARATHON = 0,
	GM_VERSUS = 1,
	GM_SPRINT40L = 2,
};

/**
* @brief Struct used to keep track of the line break animations
* /!\ To refactor soon /!\
*/
struct LineBreak { int y; Uint32 spawnTime; };

class Player
{
private:

	//[Attributes]//

	Caller_ type;
	GameMode mode;

	PlayerStatus status;

	unsigned int level;
	Uint32 gravity;

	Uint32 lastDrop;
	Uint32 lastHDrop;
	Uint32 lastMove;
	Uint32 lastRotation;
	Uint32 DASTick;
	
	Uint32 startTime, endTime;
	int score, points;

	bool DASBool;
	bool holdAllowed;
	bool backToBack;
	bool tSpin;

	unsigned int* grid; //10*20 array

	unsigned int held;

	std::vector<unsigned int> queue;

	unsigned int* typeQuantity; //7 array

	Block* currentBlock;

	SDL_Texture* UI;

	std::vector<LineBreak> lineBreakAnims;
	std::vector<int> hardDropAnims;
	int hardDropAnimLenght, hardDropBlockType;

	//[Private Methods]//

	void newBlock();

	void hold();

	void eraseFull();

	void renderGrid();

	void renderLineBreaks();

	//[Tool functions]//

	std::string chronoText(Uint32 timer) const;

	void increaseLevel();

public:

	//[Constructor & destructor]//

	Player(Caller_ type, GameMode mode, PlayerStatus defaultStatus = PS_GAME);

	~Player() { free(grid); }

	//[Public methods]//

	bool update();

	SDL_Texture* render();

};