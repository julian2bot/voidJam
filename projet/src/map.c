#include "map.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


// Grid map: 0 = road, 1 = wall, 9 = coffee/token, 3 = tree
// Arrays are allocated by initMap
SDL_Rect *walls = NULL;
SDL_Rect *items = NULL;
Vector2 *itemsPos = NULL;
int wall_count = 0;
int item_count = 0;
static int currentCellSize = 64;

void initMap(int cellSize)
{
    if (cellSize <= 0) cellSize = 64;
    currentCellSize = cellSize;
    // Count walls and items (items marked with 9)
    int countWalls = 0;
    int countItems = 0;
    for (int y = 0; y < MAP_H_; y++) {
        for (int x = 0; x < MAP_W; x++) {
            if (grid[y][x] == 1) countWalls++;
            if (grid[y][x] == 9) countItems++;
        }
    }

    // Free previous buffers if present
    if (walls) { free(walls); walls = NULL; }
    if (items) { free(items); items = NULL; }
    if (itemsPos) { free(itemsPos); itemsPos = NULL; }

    // Allocate walls and items arrays
    if (countWalls > 0) {
        walls = (SDL_Rect *)malloc(sizeof(SDL_Rect) * countWalls);
        if (!walls) {
            wall_count = 0;
            item_count = 0;
            return;
        }
    } else {
        walls = NULL;
    }

    if (countItems > 0) {
        items = (SDL_Rect *)malloc(sizeof(SDL_Rect) * countItems);
        itemsPos = (Vector2 *)malloc(sizeof(Vector2) * countItems);
        if (!items || !itemsPos) {
            if (items) { free(items); items = NULL; }
            if (itemsPos) { free(itemsPos); itemsPos = NULL; }
            item_count = 0;
        }
    } else {
        items = NULL;
        itemsPos = NULL;
    }

    // Fill walls and items
    int wi = 0;
    int ii = 0;
    for (int y = 0; y < MAP_H_; y++) {
        for (int x = 0; x < MAP_W; x++) {
            if (grid[y][x] == 1) {
                walls[wi].x = x * cellSize;
                walls[wi].y = y * cellSize;
                walls[wi].w = cellSize;
                walls[wi].h = cellSize;
                wi++;
            }
            if (grid[y][x] == 9) {
                items[ii].x = x * cellSize;
                items[ii].y = y * cellSize;
                items[ii].w = cellSize;
                items[ii].h = cellSize;
                itemsPos[ii].x = x;
                itemsPos[ii].y = y;
                ii++;
            }
        }
    }

    wall_count = wi;
    item_count = ii;
    // item_count = idx2;
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

SDL_Rect *getMapItems(void)
{
    return items;
}

int getMapItemCount(void)
{
    return item_count;
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
    if (walls) { free(walls); walls = NULL; }
    if (items) { free(items); items = NULL; }
    if (itemsPos) { free(itemsPos); itemsPos = NULL; }
    wall_count = 0;
    item_count = 0;
}

int setMapCell(int x, int y, int value)
{
    if (x < 0 || x >= MAP_W || y < 0 || y >= MAP_H_) return -1;
    grid[y][x] = value;
    return grid[y][x];
}
