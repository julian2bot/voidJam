#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <math.h>
#include <stdbool.h>
#include "gestionSDL.h"
#include "effects.h"
#include "musicPlayer.h"
#include "vector2.h"
#define SPEED_MIN 0.4f
#define SPEED_MAX 2.0f
#define FATIGUE_MIN 0.0f
#define FATIGUE_MAX 2.0f
#define ACCEL     0.06f
#define BRAKE     0.08f
#define TURN_RATE 1.5f
#define FRICTION  0.98f

#define STEER_FORCE    10.0f
#define RETURN_FORCE  14.0f
#define DAMPING        7.0f
#define MAX_WHEEL      0.8f    // ~45°
#define DELTA_TIME     0.016f
#define SCREEN_HEIGHT 600
#define SCREEN_WIDTH 1000


typedef struct player
{
    SDL_FPoint pos;
    SDL_Rect position;
    float angle;
    SDL_Texture *texture;
    float vitesse;
    float fatigue;
    float wheelAngle;     // angle du volant
    float wheelVelocity;  // inertie du volant
    int konami_index;
    bool codeKonami;
    bool tesMort;
} Player;

Player initPlayer(SDL_Renderer *renderer);
void drawPlayer(SDL_Renderer *renderer, Player player);
void movePlayer(Player *player);
void destroyPlayer(Player p);
void updatePlayer(Player *player, int turnLeft, int turnRight, int forward, int back, SDL_Rect *walls, int wall_count, SDL_Rect *items, int item_count, EffectManager *effects, MusicPlayer *playerUI, float *score);

int collision(Player *player, SDL_Rect *walls, int wall_count, SDL_Rect *intersection);
void gameOver(Player *player, EffectManager* effects);
void drawCockPit(SDL_Renderer *renderer, Player player, MusicPlayer *playerUI, SDL_Rect *walls, int wallCount, EffectManager *effects, float *score);

//dash board
static void drawSteeringWheel(SDL_Renderer *renderer, Player player);
static void drawFatigueGauge(SDL_Renderer *renderer, Player player);
static void drawSpeedGauge(SDL_Renderer *renderer, Player player);
static void drawNeedle(SDL_Renderer *renderer, int cx, int cy, int length, float angle);
static void drawDashboard(SDL_Renderer *renderer);
void drawThickLine(SDL_Renderer *r, int x1, int y1, int x2, int y2, int thickness);
void updateSteering(Player *player, int left, int right, float dt);
void gestionFatigue(Player* player, float fatigueQuantity);
void updatePlayerFatigue(Player *player, MusicPlayer *playerUI);
void drawFatigueOverlay(SDL_Renderer *renderer, Player player);

void drawArc(SDL_Renderer *r, int cx, int cy, int radius, float aStart, float aEnd);
void drawSpeedTicks(SDL_Renderer *r, int cx, int cy);
void drawFatigueArc(SDL_Renderer *r, int cx, int cy, int r1, int r2, float aStart, float aEnd);
void drawFatigueTicks(SDL_Renderer *r, int cx, int cy);
void drawMirror(SDL_Renderer* renderer);
void drawDistance(SDL_Renderer* renderer, float score);
void drawEffetMort(SDL_Renderer *renderer, EffectManager * effectsMort);
void reset_player_death_flags(void);

#endif