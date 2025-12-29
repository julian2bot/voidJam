#include "map.h"
#include <stdlib.h>
#include <string.h>

// Grid map: 0 = road, 1 = wall, 2 = token, 3 = tree
// Small example grid (mapWidth x mapHeight)
#define MAP_W 20
#define MAP_H_ 15

static int grid[MAP_H_][MAP_W] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,1},
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
    {1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

SDL_Rect *walls = NULL;
int wall_count = 0;
static int currentCellSize = 64;

void initMap(int cellSize)
{
    if (cellSize <= 0) cellSize = 64;
    currentCellSize = cellSize;

    // Count walls (cells == 1)
    int count = 0;
    for (int y = 0; y < MAP_H_; y++) {
        for (int x = 0; x < MAP_W; x++) {
            if (grid[y][x] == 1) count++;
        }
    }

    // Allocate walls array
    walls = (SDL_Rect *)malloc(sizeof(SDL_Rect) * count);
    if (!walls) {
        wall_count = 0;
        return;
    }

    // Fill walls
    int idx = 0;
    for (int y = 0; y < MAP_H_; y++) {
        for (int x = 0; x < MAP_W; x++) {
            if (grid[y][x] == 1) {
                walls[idx].x = x * cellSize;
                walls[idx].y = y * cellSize;
                walls[idx].w = cellSize;
                walls[idx].h = cellSize;
                idx++;
            }
        }
    }
    wall_count = idx;
}

int getMapCell(int x, int y)
{
    if (x < 0 || x >= MAP_W || y < 0 || y >= MAP_H_) return -1;
    return grid[y][x];
}

int getMapWidth(void)
{
    return MAP_W;
}

int getMapHeight(void)
{
    return MAP_H_;
}

int getMapCellSize(void)
{
    return currentCellSize;
}

void drawMap(SDL_Renderer *renderer, int cellSize)
{
    if (cellSize <= 0) cellSize = currentCellSize;

    // Draw cells: roads nothing, walls filled rect, tokens small yellow rect, trees green rect
    for (int y = 0; y < MAP_H_; y++) {
        for (int x = 0; x < MAP_W; x++) {
            int v = grid[y][x];
            SDL_Rect r = { x * cellSize, y * cellSize, cellSize, cellSize };
            if (v == 1) {
                SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
                SDL_RenderFillRect(renderer, &r);
            } else if (v == 2) {
                SDL_SetRenderDrawColor(renderer, 240, 200, 0, 255);
                SDL_Rect t = { r.x + cellSize/4, r.y + cellSize/4, cellSize/2, cellSize/2 };
                SDL_RenderFillRect(renderer, &t);
            } else if (v == 3) {
                SDL_SetRenderDrawColor(renderer, 30, 150, 30, 255);
                SDL_RenderFillRect(renderer, &r);
            }
        }
    }
}

void drawWalls(SDL_Renderer *renderer, SDL_Rect *wallsArr, int wall_count)
{
    SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
    for (int i = 0; i < wall_count; i++) {
        SDL_RenderFillRect(renderer, &wallsArr[i]);
    }
}