#include "map.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


// Grid map: 0 = road, 1 = wall, 2 = token, 3 = tree
// Small example grid (mapWidth x mapHeight)
#define MAP_W 20
#define MAP_H_ 15

SDL_Rect *walls = NULL;
SDL_Rect items[10];
Vector2 itemsPos[10];
int wall_count = 0;
int item_count = 0;
static int currentCellSize = 64;

void initMap(int cellSize)
{
    if (cellSize <= 0) cellSize = 64;
    currentCellSize = cellSize;

    // Count walls (cells == 1)
    int count = 0;
    int count_items = 0;
    for (int y = 0; y < MAP_H_; y++) {
        for (int x = 0; x < MAP_W; x++) {
            if (grid[y][x] == 1) count++;
            if (grid[y][x] == 9) count_items++;
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
    int idx2 = 0;
    for (int y = 0; y < MAP_H_; y++) {
        for (int x = 0; x < MAP_W; x++) {
            if (grid[y][x] == 1) {
                walls[idx].x = x * cellSize;
                walls[idx].y = y * cellSize;
                walls[idx].w = cellSize;
                walls[idx].h = cellSize;
                idx++;
            }
            if (grid[y][x] == 9) {
                items[idx2].x = x * cellSize;
                items[idx2].y = y * cellSize;
                items[idx2].w = cellSize;
                items[idx2].h = cellSize;
                itemsPos[idx2].x = x;
                itemsPos[idx2].y = y;
                idx2++;
            }
        }
    }
    wall_count = idx;
    item_count = idx2;
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
            switch (v)
            {
                case 1:
                    SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
                    SDL_RenderFillRect(renderer, &r);
                    break;
                
                case 9:
                    printf("CAFE");
                    SDL_SetRenderDrawColor(renderer, 240, 200, 0, 255);
                    SDL_Rect t = { r.x + cellSize/4, r.y + cellSize/4, cellSize/2, cellSize/2 };
                    SDL_RenderFillRect(renderer, &t);
                    break;

                // case 3:
                //     SDL_SetRenderDrawColor(renderer, 30, 150, 30, 255);
                //     SDL_RenderFillRect(renderer, &r);
                //     break;
            
                default:
                    break;
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

void freeMap(){
    free(walls);
    walls = NULL;
}