#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <math.h>
#include "gestionSDL.h"
#include "effects.h"
#define DEG2RAD (M_PI / 180.0f)
#define SPEED_MIN 0.4f
#define SPEED_MAX 2.0f
#define ACCEL     0.06f
#define BRAKE     0.08f
#define TURN_RATE 1.5f
#define FRICTION  0.98f



typedef struct player
{
    SDL_FPoint pos;
    SDL_Rect position;
    float angle;
    SDL_Texture *texture;
    float vitesse;
} Player;

Player initPlayer(SDL_Renderer *renderer);
void drawPlayer(SDL_Renderer *renderer, Player player);
void movePlayer(Player *player);
void destroyPlayer(Player p);
void updatePlayer(Player *player, int turnLeft, int turnRight, int forward, int back, SDL_Rect *walls, int wall_count, EffectManager *effects);
int collision(Player *player, SDL_Rect *walls, int wall_count, SDL_Rect *intersection);
void gameOver(Player *player);

#endif