#include "player.h"

Player initPlayer(SDL_Renderer *renderer)
{
	SDL_Rect position = {100, 100, 30, 20};
	SDL_FPoint point = {position.x, position.y};
	Player player = {point, position, 90, NULL, .4};

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

void movePlayer(Player *player)
{
	float rad = player->angle * DEG2RAD;

	player->pos.x += cosf(rad) * player->vitesse;
	player->pos.y += sinf(rad) * player->vitesse;

	player->position.x = (int)player->pos.x;
	player->position.y = (int)player->pos.y;
}

void updatePlayer(Player *player, int turnLeft, int turnRight,
                  int forward, int back, SDL_Rect *walls, int wall_count, EffectManager *effects)
{
	
	SDL_Rect intersect;
	if(collision(player, walls, wall_count, &intersect))
	{
		if(player->vitesse > 0) addExplosion(effects, intersect.x + intersect.w / 2, intersect.y + intersect.h / 2);
		printf("Collision\n");
		gameOver(player);
		return;
	}

	// Rotation
    if (turnLeft)
        player->angle += TURN_RATE;

    if (turnRight)
        player->angle -= TURN_RATE;

    // Accélération progressive (courbe)
    if (forward)
    {
        player->vitesse += (SPEED_MAX - player->vitesse) * ACCEL;
    }

    // Frein (pas de marche arrière)
    if (back)
    {
        player->vitesse -= (player->vitesse - SPEED_MIN) * BRAKE;
    }

    // Friction naturelle
    if (!forward && !back)
    {
        player->vitesse *= FRICTION;
    }

    // Clamp vitesse
    if (player->vitesse > SPEED_MAX)
        player->vitesse = SPEED_MAX;

    if (player->vitesse < SPEED_MIN)
        player->vitesse = SPEED_MIN;

    movePlayer(player);
}

int collision(Player *player, SDL_Rect *walls, int wall_count, SDL_Rect *intersection)
{
	for (int i = 0; i < wall_count; i++)
	{
		if (SDL_IntersectRect(&player->position, &walls[i], intersection))
		{
			return 1;
		}
	}
	return 0;
}
void gameOver(Player *player)
{
	player->vitesse = 0;
	
}
void destroyPlayer(Player p)
{
	destroy_textures(1, p.texture);
}
