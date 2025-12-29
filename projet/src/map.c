#include "map.h"

SDL_Rect walls[] = {

    // {270, 0, 60, 1000},
    // {0, 150, 600, 60},
    // {0, 470, 600, 60},
    // {0, 790, 600, 60},

    // {100, 150, 60, 380},
    // {100, 530, 60, 320},

    // {440, 150, 60, 380},
    // {440, 530, 60, 320},

    // {160, 300, 200, 60},
    // {240, 620, 200, 60}};

    // {0, 0, 600, 60},        // haut
    // {0, 940, 600, 60},      // bas
    // {0, 0, 60, 1000},       // gauche
    // {540, 0, 60, 1000},     // droite

    // // Bâtiments / obstacles centraux
    // {200, 200, 60, 300},
    // {340, 500, 60, 300},
    // {120, 400, 100, 60},
    // {380, 200, 100, 60},

    // // Petits blocs / obstacles internes
    // {150, 750, 120, 60},
    // {330, 100, 120, 60}


    // {0, 0, 1100, 60},      // haut
    // {0, 540, 1100, 60},    // bas
    // {0, 0, 60, 600},       // gauche
    // {1040, 0, 60, 600},    // droite

    // // Blocs centraux
    // {200, 150, 60, 300},
    // {500, 50, 60, 200},
    // {800, 350, 60, 200},
    // {350, 400, 60, 150},

    // // Obstacles internes / bâtiments
    // {150, 300, 120, 60},
    // {600, 200, 150, 60},
    // {900, 100, 150, 60}

};

int wall_count = sizeof(walls) / sizeof(SDL_Rect);

void drawWalls(SDL_Renderer *renderer, SDL_Rect *walls, int wall_count)
{

    for (int i = 0; i < wall_count; i++)
    {
        SDL_RenderFillRect(renderer, &walls[i]);
    }
}