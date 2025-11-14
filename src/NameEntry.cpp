#include "NameEntry.h"

void NameEntry::render()
{
	SDL_SetRenderTarget(Window::getWindow()->renderer, texture);

	SDL_RenderCopy(Window::getWindow()->renderer, AssetsManager::getLib()->getTexture("alpha0"), NULL, NULL);
	SDL_RenderCopy(Window::getWindow()->renderer, AssetsManager::getLib()->getTexture("entry"), NULL, NULL);

	createText(Window::getWindow()->renderer, 140, 16, "Votre nom :", AssetsManager::getLib()->getFont("futuraS"), A_N);
	std::string font;
	char buffer[2];
	buffer[1] = '\0';
	for (int i = 0; i < 3; i++) {
		if (i == cursor)
			font = "futuraM";
		else font = "futuraS";
		buffer[0] = content[i];
		createText(Window::getWindow()->renderer, 72 + (68 * i), 104, buffer,
			AssetsManager::getLib()->getFont(font), A_CENTER);
	}

	SDL_SetRenderTarget(Window::getWindow()->renderer, NULL);
}

NameEntry::NameEntry(Caller_ caller)
{
	this->caller = caller;
	cursor = 0;
	content = "AAA";

	lastCursorMove = 0;
	lastLetterMove = 0;

	texture = SDL_CreateTexture(Window::getWindow()->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 280, 176);
	toRender = true;
}

std::string NameEntry::update()
{
	Uint32 currentTick = SDL_GetTicks();
	Keyboard* keyboard = Keyboard::getKeyboard();

	if (keyboard->keyDown(KEY_RIGTH, caller) && (currentTick - lastCursorMove) > CURSOR_CD) {
		if (cursor < 2)
			cursor++;
		lastCursorMove = currentTick;
		toRender = true;
	}
	if (keyboard->keyDown(KEY_LEFT, caller) && (currentTick - lastCursorMove) > CURSOR_CD) {
		if (cursor > 0)
			cursor--;
		lastCursorMove = currentTick;
		toRender = true;
	}
	if (keyboard->keyDown(KEY_UP, caller) && (currentTick - lastLetterMove) > LETTER_CD) {
		if (content[cursor] > 'A')
			content[cursor]--;
		lastLetterMove = currentTick;
		toRender = true;
	}
	if (keyboard->keyDown(KEY_DOWN, caller) && (currentTick - lastLetterMove) > LETTER_CD) {
		if (content[cursor] < 'Z')
			content[cursor]++;
		lastLetterMove = currentTick;
		toRender = true;
	}
	return content;
}

SDL_Texture* NameEntry::getTexture()
{
	if (toRender) render();
	return texture;
}