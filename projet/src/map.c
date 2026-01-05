#include "map.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Grid map: 0 = road, 1 = wall, 9 = coffee/token, 3 = tree
// Arrays are allocated by initMap
SDL_Rect *walls = NULL;
SDL_Rect items[10];
Vector2 itemsPos[10];
int wall_count = 0;
static int currentCellSize = 64;

void initMap(int cellSize)
{
    if (cellSize <= 0) cellSize = 64;
    for (int i = 0; i < item_count; i++)
    {
        placeRandomItem(i);
    }
    
    currentCellSize = cellSize;

    // Count walls (cells == 1)
    int countWalls = 0;
    for (int y = 0; y < MAP_H_; y++) {
        for (int x = 0; x < MAP_W; x++) {
            if (grid[y][x] == 1) countWalls++;
        }
    }

    // Free previous buffers if present
    if (walls) { free(walls); walls = NULL; }

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
    wall_count = 0;
}

int setMapCell(int x, int y, int value)
{
    if (x < 0 || x >= MAP_W || y < 0 || y >= MAP_H_) return -1;
    grid[y][x] = value;
    return grid[y][x];
}

int placeRandomValue(int value) {
    int tries = 0;
    const int maxTries = 100; // éviter boucle infinie si la grille est pleine

    while (tries < maxTries) {
        int x = rand() % MAP_W;
        int y = rand() % MAP_H_;

        if (grid[y][x] == 0) {
            grid[y][x] = value;
            printf("Valeur %d placée en (%d, %d)\n", value, x, y);
            return 1; // succès
        }

        tries++;
    }

    printf("Impossible de placer la valeur : grille pleine ?\n");
    return 0; // échec
}

int placeRandomItem(int index) {
    int tries = 0;
    const int maxTries = 100; // éviter boucle infinie si la grille est pleine

    while (tries < maxTries) {
        int x = rand() % MAP_W;
        int y = rand() % MAP_H_;

        if (grid[y][x] == 0) {
            grid[y][x] = 9;
            printf("Valeur %d placée en (%d, %d)\n", 9, x, y);
            items[index].x = x * currentCellSize;
            items[index].y = y * currentCellSize;
            items[index].w = currentCellSize;
            items[index].h = currentCellSize;
            itemsPos[index].x = x;
            itemsPos[index].y = y;

            return 1; // succès
        }

        tries++;
    }

    
    printf("Impossible de placer la valeur : grille pleine ?\n");
    return 0; // échec
}