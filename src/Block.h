#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

class Block
{

private:

	//[automatic SRS import as static class attribute]//

	static std::vector<std::vector<std::vector<unsigned int>>> SRS;

	static std::vector<int> wallKicks;

	static bool SRSImported;

	static void loadSRS();

	//[instance attributes]//

	int x, y;

	unsigned int type;

	unsigned int rotation;

	unsigned int* grid;  // /!\ A pointer toward the player's grid, use caution /!\

	//[Private methods]//

	bool correctPlacement(int x_, int y_, unsigned int rotation_);

public:
	
	//[Constructor & copy constructor]//

	Block(unsigned int* grid, unsigned int type);

	Block(Block& object);

	//[Public methods]//

	unsigned int getType() const { return type; }

	bool drop();

	void hardDrop() { while (drop()); }  //Perfection !

	bool move(int direction);

	bool rotate(int direction);

	bool lodge(unsigned int* grid_);

	std::vector<int> getHeights();

	std::vector<std::vector<int>> getMolding(unsigned int* grid_);
};

std::vector<std::vector<std::vector<unsigned int>>> Block::SRS = {};
std::vector<int> Block::wallKicks = {};
bool Block::SRSImported = false;