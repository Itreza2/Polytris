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
int createText(SDL_Renderer* renderer, int x, int y, const char* content, TTF_Font* font, Anchor anchor = A_CENTER)
{
	SDL_Rect dst;
	SDL_Color white = { 0xff, 0xff, 0xff, 0xff };

	SDL_Surface* surface = TTF_RenderUTF8_Solid(font, content, white);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	switch (anchor) //dirty (._.)
	{
	case A_CENTER:
		dst = { x - surface->w / 2, y - surface->h / 2, surface->w, surface->h };
		break;
	case A_NW:
		dst = { x, y, surface->w, surface->h };
		break;
	case A_NE:
		dst = { x - surface->w, y - surface->h / 2, surface->w, surface->h };
		break;
	case A_SW:
		dst = { x, y - surface->h, surface->w, surface->h };
		break;
	case A_SE:
		dst = { x - surface->w, y - surface->h, surface->w, surface->h };
		break;
	case A_N:
		dst = { x - surface->w / 2, y, surface->w, surface->h };
		break;
	case A_S:
		dst = { x - surface->w / 2, y - surface->h, surface->w, surface->h };
		break;
	case A_W:
		dst = { x, y - surface->h / 2, surface->w, surface->h };
		break;
	case A_E:
		dst = { x - surface->w, y - surface->h / 2, surface->w, surface->h };
	}
	SDL_RenderCopy(renderer, texture, NULL, &dst);

	int lenght = surface->w;

	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);

	return lenght;
}