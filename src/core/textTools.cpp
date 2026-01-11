#include "textTools.h"

int createText(SDL_Renderer* renderer, int x, int y, const char* content, TTF_Font* font, Anchor anchor)
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