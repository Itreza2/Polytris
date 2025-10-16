#include "Block.h"

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
			file.open(("rsc\\SRS\\" + std::to_string(i) + ".txt").c_str());
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
		file.open("rsc\\SRS\\wallKick.csv");
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
			if (SRS[SRSIndex][j][i] && (y_ + j) >= 0) {

				//block out of bound
				if ((y_ + j) > 19 || (x_ + i) < 0 || (x_ + i) > 9)
					return false;
				//block overlaping on other block
				if (grid[(y_ + j) * 10 + x_ + i] && !(grid[(y_ + j) * 10 + x_ + i] == 9))
					return false;
			}
		}
	}
	return true;
}

Block::Block(unsigned int* grid, unsigned int type)
{
	loadSRS();

	x = 3;
	y = -2;
	this->grid = grid;
	this->type = type;
	rotation = 0;
}

Block::Block(Block& object)
{
	x = object.x;
	y = object.y;
	grid = object.grid;
	type = object.type;
	rotation = object.rotation;
}

bool Block::drop()
{
	if (correctPlacement(x, y + 1, rotation)) {
		y += 1;
		return true;
	}
	else
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

void Block::rotate(int direction)
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
			}
		}
	}
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
					grid_[(shadow->y + j) * 10 + shadow->x + i] = 9;
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
	return out;
}