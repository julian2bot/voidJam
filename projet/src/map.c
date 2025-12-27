#include "map.h"

SDL_Rect roads[] = {

    {270, 0, 60, 1000},
    {0, 150, 600, 60},
    {0, 470, 600, 60},
    {0, 790, 600, 60},

    {100, 150, 60, 380},
    {100, 530, 60, 320},

    {440, 150, 60, 380},
    {440, 530, 60, 320},

    {160, 300, 200, 60},
    {240, 620, 200, 60}};

int road_count = sizeof(roads) / sizeof(SDL_Rect);

void drawRoads(SDL_Renderer *renderer, SDL_Rect *roads, int road_count)
{

    for (int i = 0; i < road_count; i++)
    {
        SDL_RenderFillRect(renderer, &roads[i]);
    }
}