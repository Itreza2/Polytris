#include "AttackBuffer.h"

AttackBuffer::AttackBuffer()
{
	p1Ready = false; p2Ready = false;
	over = false;
	randomSeed = rand();
	start = 0;
	attacker = PLAYER_1;
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
	if (!garbagePile.size()) {
		attacker = caller;
		start = SDL_GetTicks();
	}
	if (caller == attacker)
		garbagePile.push_back(line);
	else
		garbagePile.erase(garbagePile.begin());
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

int AttackBuffer::getIncomingAttackPower(Caller_ caller)
{
	if (caller == attacker)
		return 0;
	else
		return garbagePile.size();
}