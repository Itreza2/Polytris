#include "Window.h"

Window::Window() {
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();

	window = SDL_CreateWindow("Polytris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, NULL);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

Window* Window::getWindow() {

	if (instance == nullptr) {
		instance = new Window();
	}
	return instance;
}

void Window::toggleFullscreen() {
	if (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN) {
		SDL_SetWindowSize(window, 1920, 1080);
		SDL_SetWindowFullscreen(window, NULL);
	}
	else {
		SDL_DisplayMode dm;
		while (SDL_GetDesktopDisplayMode(0, &dm) != 0);
		SDL_SetWindowSize(window, dm.w, dm.h);
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	}
}