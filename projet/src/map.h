#ifndef MAP_H
#define MAP_H

#include <SDL.h>

extern SDL_Rect roads[];
extern int road_count;

void drawRoads(SDL_Renderer *renderer, SDL_Rect *roads, int road_count);

#endif