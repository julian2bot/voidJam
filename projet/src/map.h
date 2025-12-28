#ifndef MAP_H
#define MAP_H

#include <SDL.h>

extern SDL_Rect walls[];
extern int wall_count;

void drawWalls(SDL_Renderer *renderer, SDL_Rect *walls, int wall_count);

#endif