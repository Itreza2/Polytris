#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>

#include "Window.h"
#include "AssetsManager.h"
#include "Keyboard.h"
#include "textTools.cpp"

const Uint32 CURSOR_CD = 300;
const Uint32 LETTER_CD = 200;

class NameEntry 
{
private:

	Caller_ caller;

	std::string content;

	int cursor;

	Uint32 lastCursorMove, lastLetterMove;

	bool toRender;

	SDL_Texture* texture;

	//[ Private methods ]//

	void render();

public:

	NameEntry(Caller_ caller);

	~NameEntry() { SDL_DestroyTexture(texture); }

	std::string update();

	SDL_Texture* getTexture();
};