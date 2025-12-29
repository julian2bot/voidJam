
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
#include "audio.h"

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

	TTF_Init();

	init(&mafenetre, &renderer);

	Player player = initPlayer(renderer);
	EffectManager effects = initEffects(renderer);
	const Uint8 *keyboard = SDL_GetKeyboardState(NULL);

	// initialize audio from folder (expects .ogg files)
	if (!audio_init_dir("assets/music")) {
		fprintf(stderr, "Warning: no audio tracks loaded from assets/music\n");
	}

	MusicPlayer playerUI = initMusicPlayer();

	// Initialize map grid and walls (cell size 64)
	initMap(64);

	Camera camera = CreateCamera(CreateVector(player.pos.x, player.pos.y), player.angle, 60);

	int fin = 0;
	while (!fin)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // fond noir
		SDL_RenderClear(renderer);


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
				// printf("touche %c\n", event.key.keysym.sym);
				if (event.key.keysym.sym == SDLK_ESCAPE) fin = 1;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					int mouseX = event.button.x;
					int mouseY = event.button.y;
					handleMusicPlayerClick(mouseX, mouseY, &playerUI);
				}
				break;
			}
		}

		// printf("fatigue : %f\n", player.fatigue);
		player.fatigue -=.001;
		CheckRays(&camera, 20, walls, wall_count, tailleFenetreW, tailleFenetreH, renderer);

		SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
		// drawWalls(renderer, walls, wall_count);
		drawCockPit(renderer, player, &playerUI, walls, wall_count, &effects);
		UpdateCameraPlayer(&camera, &player);
		// drawPlayer(renderer, player);
		updateEffects(&effects); // Update animations
		drawEffects(&effects, renderer); // Draw them

		SDL_RenderPresent(renderer);
		SDL_Delay(16);
	}

	destroyPlayer(player);
	destroyEffects(&effects);

    audio_quit();

	SDL_DestroyWindow(mafenetre);
	TTF_Quit();
	SDL_Quit();
	exit(0);
}
