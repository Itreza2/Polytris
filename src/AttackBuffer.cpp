#include "AttackBuffer.h"

AttackBuffer::AttackBuffer()
{
	p1Ready = false; p2Ready = false;
	over = false;
	start = 0;
	attacker = PLAYER_1;
	value = 0;
	garbagePile = {};
}

bool AttackBuffer::ready(Caller_ caller)
{
	if (caller == PLAYER_1)
		p1Ready = true;
	if (caller == PLAYER_2)
		p2Ready = true;
	return (p1Ready && p2Ready);
}

void AttackBuffer::attack(Caller_ caller, std::vector<int> line) 
{
	if (!value) {
		attacker = caller;
		garbagePile.clear();
		start = SDL_GetTicks();
	}
	if (caller == attacker) {
		value++;
		garbagePile.push_back(line);
	}
	else
		value--;
}

std::vector<int> AttackBuffer::collect(Caller_ caller)
{
	std::vector<int> line = {};

	if (caller != attacker && SDL_GetTicks() - start > 2000 && garbagePile.size()) {
		line = garbagePile[0];
		garbagePile.erase(garbagePile.begin());
	}
	return line;
}