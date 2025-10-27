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
	int lineCount = 0;

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
			lineCount++;
		}
	}
	if (lineCount) { //Score increase (~ standard score system minus no-line TSpins)
		float mul = 1;

		if (backToBack) mul = 1.5;
		if (tSpin) {
			points += 400 * level * (lineCount + 1) * mul;
			backToBack = true;
			attack(lineCount * 2 * mul);
		}
		else {
			if (lineCount == 4) {
				points += 800 * level * mul;
				backToBack = true;
				attack(4 * mul);
			}
			else {
				points += (100 + 200 * (lineCount - 1)) * level;
				backToBack = false;
				attack(lineCount - 1);
			}
		}
	}
	score += lineCount;
	if (score >= 5 * level && mode == GM_MARATHON) {
		score = 0;
		increaseLevel();
	}
	if (score >= 40 && mode == GM_SPRINT40L) {
		endTime = SDL_GetTicks();
		status = PS_WIN;
	}
}

void Player::attack(int power)
{
	if (mode == GM_VERSUS) {
		for (int i = 0; i < power; i++) {
			attackBuffer->attack(type, garbagePile[std::min(i, (int)garbagePile.size() - 1)]);
		}
	}
}

void Player::renderGrid()
{
	SDL_Texture* blocks = AssetsManager::getLib()->getTexture("blocks");
	SDL_Rect src, dst;
	int blockType;
	bool blockAbove;

	//Creation of the rendered grid
	unsigned int* gridCopy = (unsigned int*)malloc(sizeof(unsigned int) * 200);
	if (!gridCopy)
		throw std::exception("Memory allocation faillure");
	for (int i = 0; i < 200; i++)
		gridCopy[i] = grid[i];
	currentBlock->lodge(gridCopy);

	for (int i = 0; i < 10; i++) {
		blockAbove = false;
		for (int j = 0; j < 20; j++) {
			blockType = static_cast<int>(gridCopy[j * 10 + i]);
			if (j >= hardDropAnims[i] && !blockType && !blockAbove)
				src = { (hardDropBlockType + 1) * 64 + 4, 5, 1, 1 };
			else {
				if (blockType)
					blockAbove = true;
				src = { blockType * 64, 0, 64, 64 };
			}
			dst = { 112 + i * 32, 8 + j * 32, 32, 32 }; //112 ; 8 is the top-left corner of the board on the grid sprite
			SDL_RenderCopy(Window::getWindow()->renderer, blocks, &src, &dst);
		}
	}
	if (!hardDropAnimLenght)
		hardDropAnims.assign(10, 20);
	hardDropAnimLenght--;
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

void Player::collectGarbage()
{
	std::vector<int> line;

	while ((line = attackBuffer->collect(type)).size()) {
		for (int j = 0; j < 19; j++) {
			for (int i = 0; i < 10; i++)
				grid[j * 10 + i] = grid[(j + 1) * 10 + i];
		}
		for (int i = 0; i < 10; i++)
			grid[190 + i] = line[i];
	}
}

std::string Player::chronoText(Uint32 timer) const 
{
	return (std::to_string(timer / 60000) + "'"
		+ std::to_string(timer / 1000 % 60) + "''");
}

void Player::increaseLevel()
{
	level++;
	//Standard gravity formula, except we skip the first two levels to avoid boredom
	gravity = (Uint32)(1000 * std::pow(0.8 - ((level + 1) * 0.007), level + 1));
}

Player::Player(Caller_ type, GameMode mode, AttackBuffer* buffer, PlayerStatus defaultStatus)
{
	this->type = type;
	this->mode = mode;
	attackBuffer = buffer;
	status = defaultStatus;
	score = 0; points = 0;

	level = 0;
	increaseLevel();

	lastDrop = 0;
	lastHDrop = 0;
	lastMove = 0;
	lastRotation = 0;
	DASTick = 0;
	DASBool = false; holdAllowed = true;
	backToBack = false; tSpin = false;
	garbagePile = {};

	UI = SDL_CreateTexture(Window::getWindow()->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 576, 656);
	SDL_SetTextureBlendMode(UI, SDL_BLENDMODE_BLEND);
	lineBreakAnims = {}; hardDropAnims.assign(10, 20);
	hardDropAnimLenght = 0; hardDropBlockType = 0;

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
		collectGarbage();

		if (keyboard->keyDown(KEY_DOWN, type)) {
			if ((currentTick - lastDrop) > (gravity / 12)) {
				if (!currentBlock->drop()) {
					if (mode == GM_VERSUS)
						garbagePile = currentBlock->getMolding(grid);
					if (currentBlock->lodge(grid))
						status = PS_LOSS;
					newBlock();
				} else 
					tSpin = false;
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
				hardDropAnims = currentBlock->getHeights();
				hardDropAnimLenght = 3; //Lenght in frames of the hard drop visual effect
				hardDropBlockType = currentBlock->getType();
				currentBlock->hardDrop();
				if (mode == GM_VERSUS)
					garbagePile = currentBlock->getMolding(grid);
				if (currentBlock->lodge(grid))
					status = PS_LOSS;
				else 
					tSpin = false;
				newBlock();
				lastHDrop = currentTick;
			}
			if ((currentTick - lastDrop) > gravity) {
				if (!currentBlock->drop()) {
					if (mode == GM_VERSUS)
						garbagePile = currentBlock->getMolding(grid);
					if (currentBlock->lodge(grid))
						status = PS_LOSS;
					newBlock();
				} else
					tSpin = false;
				lastDrop = currentTick;
			}
		}
		if (keyboard->keyDown(KEY_ROTCW, type) && (currentTick - lastRotation) > ROT) {
			if (currentBlock->rotate(1))
				tSpin = true;
			lastRotation = currentTick;
		}
		if (keyboard->keyDown(KEY_ROTCCW, type) && (currentTick - lastRotation) > ROT) {
			if (currentBlock->rotate(-1))
				tSpin = true;
			lastRotation = currentTick;
		}
		if (keyboard->keyDown(KEY_FLIP, type) && (currentTick - lastRotation) > ROT) {
			if (currentBlock->rotate(2))
				tSpin = true;
			lastRotation = currentTick;
		}
		if (keyboard->keyDown(KEY_HOLD, type) && holdAllowed)
			hold();
	}
	if ((SDL_GetTicks() - startTime > 3000 || status != PS_GAME) && keyboard->keyDown(KEY_RESET, type)) {
		if (mode == GM_VERSUS && status == PS_IDLE)
			status = PS_READY;
		else if (status != PS_READY) {
			if (mode == GM_VERSUS)
				attackBuffer->declareLoss();
			return true;
		}
	}
	if (mode == GM_VERSUS) {
		if (status == PS_LOSS)
			attackBuffer->declareLoss();
		else if (attackBuffer->isOver())
			status = PS_WIN;
		if (status == PS_READY) {
			if (attackBuffer->ready(type))
				return true;
		}
		score = attackBuffer->getIncomingAttackPower(type);
	}
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
		int max;
		switch (mode) {
		case GM_MARATHON:
			max = 5 * level; 
			break;
		case GM_VERSUS:
			max = 20; break;
		case GM_SPRINT40L:
			max = 40; break;
		}
		src = { 260, 3, 1, 1 };
		dst = { 440, 8 + (int)(640 / max) * std::max(0, max - score), 24, (int)(640 / max) * std::min(max, score) };

		SDL_RenderCopy(Window::getWindow()->renderer, AssetsManager::getLib()->getTexture("blocks"), &src, &dst);
	}
	if (status != PS_IDLE && status != PS_READY) {
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
			switch (mode)
			{
			case GM_MARATHON:
				createText(Window::getWindow()->renderer, 47, 150, ("niv " + std::to_string(level)).c_str(),
					AssetsManager::getLib()->getFont("futuraM"), A_CENTER);
				createText(Window::getWindow()->renderer, 47, 186, (std::to_string(points) + "pt").c_str(),
					AssetsManager::getLib()->getFont("futuraS"), A_CENTER);
				break;
			case GM_VERSUS:
				break;
			case GM_SPRINT40L:
				createText(Window::getWindow()->renderer, 47, 150, chronoText(SDL_GetTicks() - startTime - 3000).c_str(),
					AssetsManager::getLib()->getFont("futuraM"), A_CENTER);
				createText(Window::getWindow()->renderer, 47, 186, (std::to_string(score) + "/40").c_str(),
					AssetsManager::getLib()->getFont("futuraM"), A_CENTER);
				break;
			}
		}
		break;
	case PS_LOSS:
		if (mode == GM_MARATHON) {
			createText(Window::getWindow()->renderer, 272, 258, (std::to_string(points) + "pt").c_str(),
				AssetsManager::getLib()->getFont("futuraB"), A_CENTER);
			createText(Window::getWindow()->renderer, 272, 328, "Bravo !",
				AssetsManager::getLib()->getFont("futuraH"), A_CENTER);
		}
		else {
			createText(Window::getWindow()->renderer, 272, 328, "Perdu :(",
				AssetsManager::getLib()->getFont("futuraH"), A_CENTER);
		}
		break;
	case PS_WIN:
		if (mode == GM_SPRINT40L)
			createText(Window::getWindow()->renderer, 272, 258, chronoText(endTime - startTime - 3000).c_str(),
				AssetsManager::getLib()->getFont("futuraH"), A_CENTER);
		createText(Window::getWindow()->renderer, 272, 328, "Bravo !",
			AssetsManager::getLib()->getFont("futuraH"), A_CENTER);
		break;
	case PS_READY:
		createText(Window::getWindow()->renderer, 272, 328, "Pret",
			AssetsManager::getLib()->getFont("futuraCountdown"), A_CENTER);
		break;
	}
	SDL_SetRenderTarget(Window::getWindow()->renderer, NULL);

	return UI;
}