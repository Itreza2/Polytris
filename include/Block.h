#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <SDL.h> //GetTicks, should find a workaround
#include <algorithm>

/**
* @brief Struct used to keep track of the line break animations
* /!\ To refactor soon /!\
*/
struct Reflection { int y; int x; Uint32 spawnTime; };

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

	Uint32 lockDelay, lastLockUpdate;

	bool lockState;

	//[Private methods]//

	bool correctPlacement(int x_, int y_, unsigned int rotation_);

	void updateLockDelay(bool onGround);

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

	std::vector<Reflection> getReflections();

	void bounce() { if (!correctPlacement(x, y, rotation)) y--; }

	bool locked() const { return (lockDelay); }
};