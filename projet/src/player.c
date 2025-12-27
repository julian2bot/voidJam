#include "player.h"

Player initPlayer(SDL_Renderer *renderer)
{
	SDL_Rect position = {10, 10, 20, 30};
	Player player = {position, 90, NULL};

	SDL_Texture *rectTexture = getTextureFromImage("voiture.png", renderer);

	player.texture = rectTexture;
	return player;
}

void drawPlayer(SDL_Renderer *renderer, Player player)
{
	SDL_RenderCopyEx(
		renderer,
		player.texture,
		NULL,
		&player.position,
		player.angle,
		NULL,
		SDL_FLIP_NONE);
}
