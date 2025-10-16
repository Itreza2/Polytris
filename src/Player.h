#pragma once
#include <exception>
#include <SDL.h>
#include <vector>

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

class Player
{
private:

	//[Attributes]//

	Caller_ type;

	PlayerStatus status;

	Uint32 gravity;

	Uint32 lastDrop;
	Uint32 lastHDrop;
	Uint32 lastMove;
	Uint32 lastRotation;
	Uint32 DASTick;
	
	Uint32 startTime, endTime;
	int score;

	bool DASBool;
	bool holdAllowed;

	unsigned int* grid; //10*20 array

	unsigned int held;

	std::vector<unsigned int> queue;

	unsigned int* typeQuantity; //7 array

	Block* currentBlock;

	SDL_Texture* UI;

	//[Private Methods]//

	void newBlock();

	void hold();

	void eraseFull();

	void renderGrid();

	std::string chronoText(Uint32 timer) const;

public:

	//[Constructor & destructor]//

	Player(Caller_ type, PlayerStatus defaultStatus = PS_GAME);

	~Player() { free(grid); }

	//[Public methods]//

	bool update();

	SDL_Texture* render();

};