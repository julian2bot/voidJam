
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
#include "effects.h"
#include "camera.h"

#define tailleFenetreH 600
#define tailleFenetreW 1000

// Creation de la fenetre et du canvas de la fenetre
int init(SDL_Window **window, SDL_Renderer **renderer)
{
    *window = SDL_CreateWindow(
        "Fenetre",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
        tailleFenetreW,
        tailleFenetreH,
        SDL_WINDOW_SHOWN
    );

    if (!*window)
        return 1;

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);

    if (!*renderer)
        return 1;

    SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 255);
    SDL_RenderClear(*renderer);
    SDL_RenderPresent(*renderer);

    return 0;
}


int main(int argc, char *argv[])
{
	SDL_Window *mafenetre = NULL;	// Fenetre du programme
	SDL_Event event;		// Structure pour gerer les evenements clavier, souris, joystick
	SDL_Renderer *renderer = NULL; // Canvas

	SDL_Window *mafenetre2 = NULL;
	SDL_Renderer *renderer2 = NULL; // Canvas

	TTF_Init();

	init(&mafenetre, &renderer);
	init(&mafenetre2, &renderer2);

	Player player = initPlayer(renderer);
	EffectManager effects = initEffects(renderer);
	const Uint8 *keyboard = SDL_GetKeyboardState(NULL);

	Camera camera = CreateCamera(CreateVector(player.pos.x, player.pos.y), player.angle, 60);

	int fin = 0;
	while (!fin)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // fond noir
		SDL_SetRenderDrawColor(renderer2, 0, 0, 0, 255); // fond noir
		SDL_RenderClear(renderer);
		SDL_RenderClear(renderer2);
		
		updatePlayer(&player, keyboard[SDL_SCANCODE_D], keyboard[SDL_SCANCODE_A],keyboard[SDL_SCANCODE_W], keyboard[SDL_SCANCODE_S], walls, wall_count, &effects);

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
				if (event.key.keysym.sym == SDLK_ESCAPE) fin = 1;
				break;
			}
		}

		SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

		drawWalls(renderer, walls, wall_count);
		UpdateCameraPlayer(&camera, &player);
		drawPlayer(renderer, player);
		CheckRays(&camera, 5, walls, wall_count, tailleFenetreW, tailleFenetreH, renderer, renderer2);
		updateEffects(&effects); // Update animations
		drawEffects(&effects, renderer); // Draw them
		SDL_RenderPresent(renderer);
		SDL_RenderPresent(renderer2);
		SDL_Delay(16);
	}
	
	destroyPlayer(player);
	destroyEffects(&effects);
	
	SDL_DestroyWindow(mafenetre);
	SDL_DestroyWindow(mafenetre2);
	TTF_Quit();
	SDL_Quit();
	exit(0);
}
