#pragma once
#include <exception>
#include <SDL.h>
#include <vector>

#include "Block.h"
#include "Window.h"
#include "Keyboard.h"
#include "AssetsManager.h"

const Uint32 ARR = 100;
const Uint32 DAS = 200;
const Uint32 HD_Lock = 250;
const Uint32 ROT = 400;

class Player
{
private:

	//[Attributes]//

	Caller_ type;

	Uint32 gravity;

	Uint32 lastDrop;
	Uint32 lastHDrop;
	Uint32 lastMove;
	Uint32 lastRotation;
	Uint32 DASTick;
	
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

public:

	//[Constructor & destructor]//

	Player(Caller_ type);

	~Player() { free(grid); }

	//[Public methods]//

	bool update();

	SDL_Texture* render();

};