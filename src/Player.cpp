#include "Player.h"

void Player::newBlock()
{
	eraseFull();
	bool randomCtr = false;
	unsigned int newType;

	if (currentBlock)
		free(currentBlock);
	currentBlock = new Block(grid, queue.front());
	typeQuantity[queue.front()]--;
	queue.erase(queue.begin());

	while (!randomCtr) {
		newType = rand() % 7;
		if (typeQuantity[newType] < 2)
			randomCtr = true;
	}
	typeQuantity[newType]++;
	queue.push_back(newType);

	holdAllowed = true;
}

void Player::hold()
{
	unsigned int currentType = currentBlock->getType();

	if (held == 7) {
		newBlock();
		held = currentType;
	}
	else {
		free(currentBlock);
		currentBlock = new Block(grid, held);
		held = currentType;
	}
	holdAllowed = false;
}

void Player::eraseFull()
{
	bool full;

	for (int j = 0; j < 20; j++) {
		full = true;
		for (int i = 0; i < 10; i++) {
			if (grid[j * 10 + i] == 0)
				full = false;
		}
		if (full) {
			for (int line = j; line > 1; line--) {
				for (int i = 0; i < 10; i++)
					grid[line * 10 + i] = grid[(line - 1) * 10 + i];
			}
			for (int i = 0; i < 10; i++)
				grid[i] = 0;
		}
	}
}

Player::Player(Caller_ type) 
{
	this->type = type;

	gravity = 400;
	lastDrop = 0;
	lastHDrop = 0;
	lastMove = 0;
	lastRotation = 0;
	DASTick = 0;
	DASBool = false;
	holdAllowed = true;

	UI = SDL_CreateTexture(Window::getWindow()->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 16 * 10, 16 * 20);

	grid = (unsigned int*)malloc(sizeof(unsigned int) * 200);
	typeQuantity = (unsigned int*)malloc(sizeof(unsigned int) * 7);
	if (!grid || !typeQuantity)
		throw std::exception("Memory allocation faillure");
	for (int i = 0; i < 200; i++)
		grid[i] = 0;
	for (int i = 0; i < 7; i++)
		typeQuantity[i] = 1;  //One of each piece in the starting queue

	held = 7;

	//Initialisation of the queue with 7-bag
	std::vector<unsigned int> bag = { 0, 1, 2, 3, 4, 5, 6 };
	while (!bag.empty()) {
		size_t index = rand() % bag.size();
		queue.push_back(bag[index]);
		bag.erase(bag.begin() + index);
	}
	newBlock();
}

bool Player::update()
{
	Keyboard* keyboard = Keyboard::getKeyboard();
	Uint32 currentTick = SDL_GetTicks();

	if (keyboard->keyDown(KEY_DOWN, type)) {
		if ((currentTick - lastDrop) > (gravity / 12)) {
			if (!currentBlock->drop()) {
				currentBlock->lodge(grid);
				newBlock();
			}
			lastDrop = currentTick;
		}
	}
	else {
		if (!keyboard->keyDown(KEY_RIGTH, type) && !keyboard->keyDown(KEY_LEFT, type)) {
			DASTick = currentTick;
			DASBool = false;
		}
		else {
			Uint32 cooldown;
			if ((currentTick - DASTick) < DAS && DASBool)
				lastMove = currentTick;
			if (keyboard->keyDown(KEY_RIGTH, type) && (currentTick - lastMove) > ARR) {
				currentBlock->move(1);
				lastMove = currentTick;
				DASBool = true;
			}
			if (keyboard->keyDown(KEY_LEFT, type) && (currentTick - lastMove) > ARR) {
				currentBlock->move(-1);
				lastMove = currentTick;
				DASBool = true;
			}
		}
		if (keyboard->keyDown(KEY_HDROP, type) && (currentTick - lastHDrop) > HD_Lock) {
			currentBlock->hardDrop();
			currentBlock->lodge(grid);
			newBlock();
			lastHDrop = currentTick;
		}
		if ((currentTick - lastDrop) > gravity) {
			if (!currentBlock->drop()) {
				currentBlock->lodge(grid);
				newBlock();
			}
			lastDrop = currentTick;
		}
	}
	if (keyboard->keyDown(KEY_ROTCW, type) && (currentTick - lastRotation) > ROT) {
		currentBlock->rotate(1);
		lastRotation = currentTick;
	}
	if (keyboard->keyDown(KEY_ROTCCW, type) && (currentTick - lastRotation) > ROT) {
		currentBlock->rotate(-1);
		lastRotation = currentTick;
	}
	if (keyboard->keyDown(KEY_FLIP, type) && (currentTick - lastRotation) > ROT) {
		currentBlock->rotate(2);
		lastRotation = currentTick;
	}
	if (keyboard->keyDown(KEY_HOLD, type) && holdAllowed)
		hold();
	if (keyboard->keyDown(KEY_RESET, type))
		return true;

	return false;
}

SDL_Texture* Player::render()
{
	SDL_Texture* blocks = AssetsManager::getLib()->getTexture("blocks");
	SDL_Rect src, dst;
	int blockType;

	//Creation of the rendered grid
	unsigned int* gridCopy = (unsigned int*)malloc(sizeof(unsigned int) * 200);
	if (!gridCopy)
		throw std::exception("Memory allocation faillure");
	for (int i = 0; i < 200; i++)
		gridCopy[i] = grid[i];
	currentBlock->lodge(gridCopy);
	
	//Filling of the Surface
	SDL_SetRenderTarget(Window::getWindow()->renderer, UI);
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 20; j++) {
			blockType = static_cast<int>(gridCopy[j * 10 + i]);
			src = { blockType * 16, 0, 16, 16 };
			dst = { i * 16, j * 16, 16, 16 };
			SDL_RenderCopy(Window::getWindow()->renderer, blocks, &src, &dst);
			std::cout << SDL_GetError() << std::endl;
		}
	}
	SDL_SetRenderTarget(Window::getWindow()->renderer, NULL);

	return UI;
}