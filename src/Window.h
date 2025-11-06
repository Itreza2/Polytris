#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

class Window
{
/**
* A Singleton that initialize and handle a SDL window and its rendering context
*/
private:

	static Window* instance;

	Window();

public:

	//Might as well be public, SDL works this way
	SDL_Window* window;
	SDL_Renderer* renderer;

	static Window* getWindow();

	void toggleFullscreen();

};

Window* Window::instance = nullptr;