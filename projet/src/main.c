
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include <time.h>
#include <SDL_ttf.h>
#include "player.h"
#include "map.h"
#include "gestionSDL.h"

#define tailleFenetreH 600
#define tailleFenetreW 1000

// Creation de la fenetre et du canvas de la fenetre
int init(SDL_Window **mafenetre, SDL_Renderer *canvas, SDL_Renderer **renderer)
{
	int res = 0;
	if (SDL_VideoInit(NULL) < 0)
		res = 1; // SDL_VideoInit renvoie 0 en cas de succes
	SDL_CreateWindowAndRenderer(tailleFenetreW, tailleFenetreH, SDL_WINDOW_SHOWN, mafenetre, renderer);
	SDL_SetRenderDrawColor(canvas, 0, 0, 0, 255);
	SDL_RenderClear(canvas);
	return res;
}

int main(int argc, char *argv[])
{
	SDL_Window *mafenetre;	// Fenetre du programme
	SDL_Event event;		// Structure pour gerer les evenements clavier, souris, joystick
	SDL_Renderer *renderer; // Canvas

	TTF_Init();

	init(&mafenetre, renderer, &renderer);

	Player player = initPlayer(renderer);

	int fin = 0;
	while (!fin)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // fond noir
		SDL_RenderClear(renderer);

		// Boucle principale
		if (SDL_PollEvent(&event))
		{ // Scrute sans cesse les evenements et renvoie 1
			switch (event.type)
			{
			case SDL_QUIT: // Evenement fermeture de la fenetre
				fin = 1;
				break;

			case SDL_KEYDOWN:
				printf("touche %c\n", event.key.keysym.sym);
				if (event.key.keysym.sym == SDLK_ESCAPE)
					fin = 1;
				break;
			}
		}

		SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

		drawRoads(renderer, roads, road_count);
		drawPlayer(renderer, player);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyWindow(mafenetre);
	TTF_Quit();
	SDL_Quit();
	exit(0);
}
