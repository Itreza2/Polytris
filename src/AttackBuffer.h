#pragma once
#include <vector>

#include "Keyboard.h" // Weird but justified

class AttackBuffer
{
public:

	Uint32 randomSeed;

private:

	bool p1Ready, p2Ready, over;

	Uint32 start;

	Caller_ attacker;

	std::vector<std::vector<int>> garbagePile;

public:

	AttackBuffer();

	bool ready(Caller_ caller);

	void attack(Caller_ caller, std::vector<int> line);

	std::vector<int> collect(Caller_ caller);

	void declareLoss() { over = true; }

	bool isOver() const { return over; }

	int getIncomingAttackPower(Caller_ caller);

};