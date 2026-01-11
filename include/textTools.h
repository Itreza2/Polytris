#pragma once
#include <SDL.h>
#include <SDL_ttf.h>

enum Anchor
{
	A_CENTER,
	A_NW,
	A_NE,
	A_SW,
	A_SE,
	A_N,
	A_S,
	A_W,
	A_E,
};

/**
* @brief render text on a rendering context
* @param renderer the root SDL renderer on which the text will be applied
* @param x the coordinate of the anchor on the x axis in px
* @param y the coordinate of the anchor on the y axix in px
* @param content the text to be rendered
* @param font the ttf font that will be used to render the text
* @param anchor the type of anchoring used to position the text
*/
int createText(SDL_Renderer* renderer, int x, int y, const char* content, TTF_Font* font, Anchor anchor = A_CENTER);