#include "Block.h"

std::vector<std::vector<std::vector<unsigned int>>> Block::SRS = {};
std::vector<int> Block::wallKicks = {};
bool Block::SRSImported = false;

//Static Method
void Block::loadSRS() 
{
	if (!SRSImported) {
		SRSImported = true;

		std::ifstream file;
		std::string line, cell;
		std::stringstream separatedLine;

		SRS = {};									//SRS vector filling
		for (int i = 0; i <= 6; i++) {
			file.open(("rsc/SRS/" + std::to_string(i) + ".txt").c_str());
			if (file.is_open()) {
				SRS.push_back({});
				while (std::getline(file, line)) {
					if (line == "next") {
						SRS.push_back({});
					}
					else {
						SRS[SRS.size() - 1].push_back({});
						for (char c : line) {
							if (c >= '0' && c <= '9') {
								SRS[SRS.size() - 1][SRS[SRS.size() - 1].size() - 1].push_back(c - '0');
							}
						}
					}
				}
				file.close();
			}
			else {
				std::cout << "fstream failure" << std::endl;
				//TODO : fstream failure handling...
			}
		}
		wallKicks = {};								//Wall kicks vector filling
		file.open("rsc/SRS/wallKick.csv");
		if (file.is_open()) {
			SRS.push_back({});
			while (std::getline(file, line)) {
				separatedLine = std::stringstream(line);
				while (std::getline(separatedLine, cell, ','))
				{
					wallKicks.push_back(stoi(cell));
				}
			}
			file.close();
		}
		else {
			std::cout << "fstream failure" << std::endl;
			//TODO : fstream failure handling...
		}
	}
}

bool Block::correctPlacement(int x_, int y_, unsigned int rotation_)
{
	unsigned int SRSIndex = type * 4 + rotation_;
	int shapeSize = static_cast<int>(SRS[SRSIndex].size());

	for (int i = 0; i < shapeSize; i++) {
		for (int j = 0; j < shapeSize; j++) {
			if (SRS[SRSIndex][j][i]) {

				//block out of bound
				if ((y_ + j) > 19 || (x_ + i) < 0 || (x_ + i) > 9)
					return false;
				//block overlaping on other block
				if ((y_ + j) >= 0) {
					if (grid[(y_ + j) * 10 + x_ + i] && !(grid[(y_ + j) * 10 + x_ + i] == 9))
						return false;
				}
			}
		}
	}
	return true;
}

void Block::updateLockDelay(bool onGround)
{
	if (onGround) {
		if (lockState)
			lockDelay -= std::min(lockDelay, SDL_GetTicks() - lastLockUpdate);
		lastLockUpdate = SDL_GetTicks();
	}
	lockState = onGround;
}

Block::Block(unsigned int* grid, unsigned int type)
{
	loadSRS();

	x = 3;
	y = -2;
	this->grid = grid;
	this->type = type;
	rotation = 0;
	
	lastLockUpdate = 0;
	lockDelay = 500;
	lockState = false;
}

Block::Block(Block& object)
{
	x = object.x;
	y = object.y;
	grid = object.grid;
	type = object.type;
	rotation = object.rotation;

	lastLockUpdate = 0;
	lockDelay = 500;
	lockState = false;
}

bool Block::drop()
{
	if (correctPlacement(x, y + 1, rotation)) {
		y += 1;
		updateLockDelay(false);
		return true;
	}
	else
		updateLockDelay(true);
		return false;
}

bool Block::move(int direction)
{
	if (correctPlacement(x + direction, y, rotation)) {
		x += direction;
		return true;
	}
	else
		return false;
}

bool Block::rotate(int direction)
{
	if (direction != 2 || (type != 0 && type != 4 && type != 6)) {
		int newDirection;
		if (rotation + direction < 0)
			newDirection = 3;
		else
			newDirection = (rotation + direction) % 4;

		if (direction == 2) {
			if (correctPlacement(x, y, newDirection))
				rotation = newDirection;
		}
		else {
			unsigned int failures = 0;
			unsigned int t; //No name in mind...
			if (direction == 1)
				t = 0;
			else
				t = 4;
			while (!correctPlacement(
				x + wallKicks[(8 * type + t + newDirection) * 10 + failures * 2],
				y + wallKicks[(8 * type + t + newDirection) * 10 + failures * 2 + 1],
				newDirection) && failures < 4)
					failures++;
			if (failures < 4) {
				rotation = newDirection;
				x += wallKicks[(8 * type + t + rotation) * 10 + failures * 2];
				y += wallKicks[(8 * type + t + rotation) * 10 + failures * 2 + 1];
				if (type == 5)
					return true;
			}
		}
	}
	return false;
}

bool Block::lodge(unsigned int* grid_)
{
	Block* shadow = new Block(*this);
	shadow->hardDrop();

	unsigned int SRSIndex = type * 4 + rotation;
	int shapeSize = static_cast<int>(SRS[SRSIndex].size());
	bool out = false;

	for (int i = 0; i < shapeSize; i++) {
		for (int j = 0; j < shapeSize; j++) {

			//Block's shadow
			if (SRS[SRSIndex][j][i]) {
				if (shadow->y + j >= 0 && shadow->y + j <= 19 && shadow->x + i >= 0 && shadow->x + i <= 9) {
					grid_[(shadow->y + j) * 10 + shadow->x + i] = type + 9;
				}
			}
			//Block itself
			if (SRS[SRSIndex][j][i]) {
				if (y + j >= 0 && y + j <= 19 && x + i >= 0 && x + i <= 9) {
					grid_[(y + j) * 10 + x + i] = type + 1;
				}
				else out = true;
			}
		}
	}
	delete shadow;
	return out;
}

std::vector<int> Block::getHeights()
{
	std::vector<int> returnVector;
	returnVector.assign(10, 20);

	unsigned int SRSIndex = type * 4 + rotation;
	int shapeSize = static_cast<int>(SRS[SRSIndex].size());

	for (int i = 0; i < shapeSize; i++) {
		for (int j = 0; j < shapeSize; j++) {

			if (SRS[SRSIndex][j][i]) {
				if (x + i >= 0 && x + i <= 9)
					returnVector[x + i] = y + j;
			}
		}
	}
	return returnVector;
}

std::vector<std::vector<int>> Block::getMolding(unsigned int* grid_)
{
	unsigned int SRSIndex = type * 4 + rotation;
	int shapeSize = static_cast<int>(SRS[SRSIndex].size());
	bool full;
	std::vector<std::vector<int>> lines = {};

	for (int j = 0; j < shapeSize; j++) {
		full = true;
		for (int i = 0; i < 10; i++) {
			if (!grid[(j + y) * 10 + i]) {
				if (i < x || i - x > shapeSize - 1)
					full = false;
				else if (!SRS[SRSIndex][j][i - x])
					full = false;
			}
		}
		if (full) {
			lines.push_back({});
			for (int i = 0; i < 10; i++) {
				if (grid[(j + y) * 10 + i])
					lines[lines.size() - 1].push_back(8);
				else
					lines[lines.size() - 1].push_back(0);
			}
		}
	}
	return lines;
}

std::vector<Reflection> Block::getReflections()
{
	unsigned int SRSIndex = type * 4 + rotation;
	int shapeSize = static_cast<int>(SRS[SRSIndex].size());
	std::vector<Reflection> reflections = {};

	for (int j = 0; j < shapeSize; j++) {
		for (int i = 0; i < shapeSize; i++) {
			if (i + x >= 0 && i + x < 10 && j + y >= 0 && j + y < 20
				&& SRS[SRSIndex][j][i]) {
				reflections.push_back({ i + x, j + y, SDL_GetTicks() });
			}
		}
	}
	return reflections;
}