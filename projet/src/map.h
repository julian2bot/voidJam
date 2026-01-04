#ifndef MAP_H
#define MAP_H

#include <SDL.h>

#define MAP_W 20
#define MAP_H_ 15


extern SDL_Rect *walls;
extern int wall_count;

void initMap(int cellSize);

int getMapCell(int x, int y);

void drawMap(SDL_Renderer *renderer, int cellSize);

void drawWalls(SDL_Renderer *renderer, SDL_Rect *wallsArr, int wall_count);


// Map helpers
int getMapWidth(void);
int getMapHeight(void);
int getMapCellSize(void);
#endif