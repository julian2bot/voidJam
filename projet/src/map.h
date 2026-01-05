#ifndef MAP_H
#define MAP_H

#include <SDL.h>
#include "vector2.h"

#define MAP_W 20
#define MAP_H_ 15

static int grid[MAP_H_][MAP_W] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

typedef struct itemVisualisation{
    int type; // cofee 9
    Vector2 position;
}ItemVisualisation;

extern SDL_Rect *walls;
extern int wall_count;
extern SDL_Rect items[10];
extern Vector2 itemsPos[10];
static int item_count = 10;

void initMap(int cellSize);

int getMapCell(int x, int y);
int placeRandomValue(int value);
int placeRandomItem(int index);
void drawMap(SDL_Renderer *renderer, int cellSize);

void drawWalls(SDL_Renderer *renderer, SDL_Rect *wallsArr, int wall_count);

/* Items helpers */
SDL_Rect *getMapItems(void);
int getMapItemCount(void);

void freeMap();

// Map helpers
int getMapWidth(void);
int getMapHeight(void);
int getMapCellSize(void);
int setMapCell(int x, int y, int value);
#endif