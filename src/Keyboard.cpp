#include "Keyboard.h"

Keyboard::Keyboard() 
{
	keyMapping = (unsigned int*)malloc(sizeof(unsigned int) * 21);
	if (keyMapping != NULL) {
		keyMapping[0] = SDL_SCANCODE_ESCAPE;
		keyMapping[1] = SDL_SCANCODE_UP;
		keyMapping[2] = SDL_SCANCODE_DOWN;
		keyMapping[3] = SDL_SCANCODE_RIGHT;
		keyMapping[4] = SDL_SCANCODE_LEFT;
		keyMapping[5] = SDL_SCANCODE_R;
		keyMapping[6] = SDL_SCANCODE_T;
		keyMapping[7] = SDL_SCANCODE_Y;
		keyMapping[8] = SDL_SCANCODE_F;
		keyMapping[9] = SDL_SCANCODE_G;
		keyMapping[10] = SDL_SCANCODE_H;
		keyMapping[11] = SDL_SCANCODE_W;
		keyMapping[12] = SDL_SCANCODE_S;
		keyMapping[13] = SDL_SCANCODE_D;
		keyMapping[14] = SDL_SCANCODE_A;
		keyMapping[15] = SDL_SCANCODE_U;
		keyMapping[16] = SDL_SCANCODE_I;
		keyMapping[17] = SDL_SCANCODE_O;
		keyMapping[18] = SDL_SCANCODE_J;
		keyMapping[19] = SDL_SCANCODE_K;
		keyMapping[20] = SDL_SCANCODE_L;
	}
	isPressed = (bool*)malloc(sizeof(bool) * 21);
	if (isPressed != NULL) {
		for (int i = 0; i < 21; i++) isPressed[i] = false;
	}
}

Keyboard* Keyboard::getKeyboard() 
{
	
	if (instance == nullptr) {
		instance = new Keyboard();
	}
	return instance;
}

bool Keyboard::catchEvents() 
{
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			return true;
		} if (event.type == SDL_KEYDOWN) {
			for (int i = 0; i < 21; i++) {
				if (keyMapping[i] == event.key.keysym.scancode)
					isPressed[i] = true;
			}
		} if (event.type == SDL_KEYUP) {
			for (int i = 0; i < 21; i++) {
				if (keyMapping[i] == event.key.keysym.scancode) 
					isPressed[i] = false;
			}
		}
	}
	return false;
}

bool Keyboard::keyDown(Key_ keyName, Caller_ caller) 
{
	if (caller == MAIN_MENU) {
		switch (keyName) {
			case (KEY_EXIT):
				return isPressed[0];
				break;
			case (KEY_ANY):
				for (unsigned int i = 5; i <= 10; i++) {
					if (isPressed[i] || isPressed[i + 10])
						return true;
				} return false;
				break;
			default:
				return isPressed[keyName] || isPressed[keyName + 10];
		};
	}
	if (caller == PLAYER_1 && keyName != KEY_EXIT && keyName != KEY_ANY)
		return isPressed[keyName];
	if (caller == PLAYER_2 && keyName != KEY_EXIT && keyName != KEY_ANY)
		return isPressed[keyName + 10];
	return false; //should never be called
}