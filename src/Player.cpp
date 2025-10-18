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
			lineBreakAnims.push_back({j, SDL_GetTicks()});
			score++;
		}
	}
	if (score >= 40) {
		endTime = SDL_GetTicks();
		status = PS_WIN;
	}
}

void Player::renderGrid()
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

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 20; j++) {
			blockType = static_cast<int>(gridCopy[j * 10 + i]);
			src = { blockType * 64, 0, 64, 64 };
			dst = { 112 + i * 32, 8 + j * 32, 32, 32 }; //112 ; 8 is the top-left corner of the board on the grid sprite
			SDL_RenderCopy(Window::getWindow()->renderer, blocks, &src, &dst);
		}
	}
}

void Player::renderLineBreaks()
{
	SDL_Rect src, dst;
	LineBreak lineBreak;
	std::vector<int> endedAnims;

	for (int index = 0; index < lineBreakAnims.size(); index++) {
		lineBreak = lineBreakAnims[index];

		src = { 64 * (int)((SDL_GetTicks() - lineBreak.spawnTime) / 50 % 12), 0, 64, 64 };
		for (int i = 0; i < 10; i++) {
			dst = { 110 + 32 * i, 6 + lineBreak.y * 32, 36, 36 };
			SDL_RenderCopy(Window::getWindow()->renderer, AssetsManager::getLib()->getTexture("smoke"), &src, &dst);
		}
		if ((SDL_GetTicks() - lineBreak.spawnTime) / 50 % 12 > 10) {
			endedAnims.push_back(index);
		}
	}
	for (int index = endedAnims.size() - 1; index >= 0; index--) {
		lineBreakAnims.erase(lineBreakAnims.begin() + endedAnims[index]);
	}
}

std::string Player::chronoText(Uint32 timer) const 
{
	return (std::to_string(timer / 60000) + "'"
		+ std::to_string(timer / 1000 % 60) + "''");
}

Player::Player(Caller_ type, PlayerStatus defaultStatus)
{
	this->type = type;
	status = defaultStatus;
	score = 0;

	gravity = 400;
	lastDrop = 0;
	lastHDrop = 0;
	lastMove = 0;
	lastRotation = 0;
	DASTick = 0;
	DASBool = false;
	holdAllowed = true;

	UI = SDL_CreateTexture(Window::getWindow()->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 576, 656);
	SDL_SetTextureBlendMode(UI, SDL_BLENDMODE_BLEND);
	lineBreakAnims = {};

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

	endTime = startTime = SDL_GetTicks();;
}

bool Player::update()
{
	Keyboard* keyboard = Keyboard::getKeyboard();
	Uint32 currentTick = SDL_GetTicks();

	if (SDL_GetTicks() - startTime > 3000 && status == PS_GAME) {
		if (keyboard->keyDown(KEY_DOWN, type)) {
			if ((currentTick - lastDrop) > (gravity / 12)) {
				if (!currentBlock->drop()) {
					if (currentBlock->lodge(grid))
						status = PS_LOSS;
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
				if (currentBlock->lodge(grid))
					status = PS_LOSS;
				newBlock();
				lastHDrop = currentTick;
			}
			if ((currentTick - lastDrop) > gravity) {
				if (!currentBlock->drop()) {
					if (currentBlock->lodge(grid))
						status = PS_LOSS;
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
	}
	if ((SDL_GetTicks() - startTime > 3000 || status != PS_GAME) && keyboard->keyDown(KEY_RESET, type))
		return true;
	return false;
}

SDL_Texture* Player::render()
{
	SDL_Rect src, dst;
	int blockType;
	
	SDL_SetRenderTarget(Window::getWindow()->renderer, UI);

	//Overwrite previous state with the blank board texture, could be made in a cleaner way
	SDL_RenderCopy(Window::getWindow()->renderer, AssetsManager::getLib()->getTexture("grid"), NULL, NULL);
	
	if (held < 7) { //Held piece preview
		src = { 50 * (int)held, 0, 50, 50 };
		dst = { 8, 8, 94, 94 };
		SDL_RenderCopy(Window::getWindow()->renderer, AssetsManager::getLib()->getTexture("blocksPreview"), &src, &dst);
	} 
	if (score) { //Score bar filling
		src = { 260, 3, 1, 1 };
		dst = { 440, 8 + 16 * std::max(0, 40 - score), 24, 16 * std::min(40, score)};
		SDL_RenderCopy(Window::getWindow()->renderer, AssetsManager::getLib()->getTexture("blocks"), &src, &dst);
	}
	if (status != PS_IDLE) {
		for (int i = 0; i < 5; i++) { //Bag preview
			src = { 50 * (int)queue[i], 0, 50, 50 };
			dst = { 472, 8 + i * 94, 94, 94 };
			SDL_RenderCopy(Window::getWindow()->renderer, AssetsManager::getLib()->getTexture("blocksPreview"), &src, &dst);
		}
		renderGrid();
		renderLineBreaks();
	}

	switch (status)
	{
	case PS_IDLE:
		if ((SDL_GetTicks() - startTime) / 500 % 2)
		createText(Window::getWindow()->renderer, 272, 328, "En attente...",
			AssetsManager::getLib()->getFont("futuraB"), A_CENTER);
		break;
	case PS_GAME:
		if (SDL_GetTicks() - startTime < 3000) {
			createText(Window::getWindow()->renderer, 272, 328, std::to_string(3 - ((SDL_GetTicks() - startTime) / 1000)).c_str(),
				AssetsManager::getLib()->getFont("futuraCountdown"), A_CENTER);
		}
		else {
			createText(Window::getWindow()->renderer, 47, 150, chronoText(SDL_GetTicks() - startTime - 3000).c_str(),
				AssetsManager::getLib()->getFont("futuraM"), A_CENTER);
			createText(Window::getWindow()->renderer, 47, 186, (std::to_string(score) + "/40").c_str(),
				AssetsManager::getLib()->getFont("futuraM"), A_CENTER);
		}
		break;
	case PS_LOSS:
		createText(Window::getWindow()->renderer, 272, 328, "Perdu :(",
			AssetsManager::getLib()->getFont("futuraH"), A_CENTER);
		break;
	case PS_WIN:
		createText(Window::getWindow()->renderer, 272, 258, chronoText(endTime - startTime - 3000).c_str(),
			AssetsManager::getLib()->getFont("futuraH"), A_CENTER);
		createText(Window::getWindow()->renderer, 272, 328, "Bravo !",
			AssetsManager::getLib()->getFont("futuraH"), A_CENTER);
		break;
	}
	SDL_SetRenderTarget(Window::getWindow()->renderer, NULL);

	return UI;
}