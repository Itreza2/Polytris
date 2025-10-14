#pragma once
#include <iostream>
#include <SDL.h>

enum Key_ {
	KEY_EXIT = 0,
	KEY_UP = 1,
	KEY_DOWN = 2,
	KEY_RIGTH = 3,
	KEY_LEFT = 4,
	KEY_ROTCCW = 5,
	KEY_FLIP = 6,
	KEY_ROTCW = 7,
	KEY_HDROP = 8,
	KEY_HOLD = 9,
	KEY_RESET = 10,
	KEY_ANY = 11
}; //Note : Player 2 use the same macros than Player 1

enum Caller_ {
	MAIN_MENU,
	PLAYER_1,
	PLAYER_2
};

class Keyboard
{
private:

	static Keyboard* instance;

	unsigned int* keyMapping;

	bool* isPressed;

	SDL_Event event;

	Keyboard();

public:

	static Keyboard* getKeyboard();

	bool catchEvents();

	bool keyDown(Key_ keyName, Caller_ caller = MAIN_MENU);
};

Keyboard* Keyboard::instance = nullptr;