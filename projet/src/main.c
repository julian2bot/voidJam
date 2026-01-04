
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <time.h>
#include "player.h"
#include "map.h"
#include "gestionSDL.h"
#include "effects.h"
#include "camera.h"
#include "button.h"

#define tailleFenetreH 600
#define tailleFenetreW 1000

#define buttonWidth 200
#define	buttonHeight 50

typedef enum State{
    MAIN,
    GAME,
    GAME_OVER,
    EXIT
}State;

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

State game(Player player, SDL_Renderer *renderer, const Uint8 *keyboard, SDL_Event event, EffectManager effects){
	MusicPlayer playerUI = initMusicPlayer();

	Camera camera = CreateCamera(CreateVector(player.pos.x, player.pos.y), player.angle, 60);
	int fin = 0;
	while (!player.tesMort && !fin)
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
		CheckRays(&camera, 5, walls, wall_count, tailleFenetreW, tailleFenetreH, renderer);
		
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
	if(fin){
		return EXIT;
	}
	return GAME_OVER;
}

State MainMenu(SDL_Renderer *renderer, SDL_Event event){
	TTF_Font *font = TTF_OpenFont("time.ttf", 32);
	TTF_Font *fontTitle = TTF_OpenFont("time.ttf", 100);

	SDL_Rect rectTitle = {0,20, tailleFenetreW, 50};
	SDL_Rect rectStart = {tailleFenetreW/2 - buttonWidth/2, tailleFenetreH/2 - buttonHeight/2, buttonWidth, buttonHeight};
	SDL_Rect rectExit = {tailleFenetreW/2 - buttonWidth/2, tailleFenetreH/2 - buttonHeight/2 + 100, buttonWidth, buttonHeight};
	SDL_Color white = {255,255,255,255};
	SDL_Color lightBlue = {18,247,255,255};
	SDL_Color black = {0,0,0,255};
	Button buttonStart = CreateButton(rectStart,white, lightBlue, "Start", black, black);
	Button buttonExit = CreateButton(rectExit,white, lightBlue, "Exit", black, black);
	SetSelected(&buttonStart, 1);

	Button buttons[2] = {buttonStart, buttonExit};

	int index = 0;
	
	State currentState = MAIN;
	
	while (currentState == MAIN)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // fond noir
		SDL_RenderClear(renderer);

		// Boucle principale
		if (SDL_PollEvent(&event))
		{ // Scrute sans cesse les evenements et renvoie 1
			switch (event.type)
			{
				case SDL_QUIT: // Evenement fermeture de la fenetre
					currentState = EXIT;
					break;

				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							currentState = EXIT;
							break;

						case SDLK_z:
						case SDLK_UP:
							buttons[index].selected = 0;
							// index++;
							// if(index>=2){
							// 	index = 0;
							// }
							index = !index;
							buttons[index].selected = 1;
							break;
						
						case SDLK_s:
						case SDLK_DOWN:
							buttons[index].selected = 0;
							// index--;
							// if(index<0){
							// 	index = 1;
							// }
							index = !index;
							buttons[index].selected = 1;
							break;

						case SDLK_KP_ENTER:
						case SDLK_RETURN:
							if(index==0){currentState=GAME;}
							else{currentState=EXIT;}
							break;
						
						default:
							break;
					}
					break;
			}
		}

		for (int i = 0; i < 2; i++)
		{
			displayButton(&buttons[i], renderer, font);
		}
		drawCenteredText(renderer, fontTitle, white, "TITLE", rectTitle);

		SDL_RenderPresent(renderer);
	}
	return currentState;
}

State GameOverMenu(SDL_Renderer *renderer, SDL_Event event){
	TTF_Font *font = TTF_OpenFont("time.ttf", 32);
	TTF_Font *fontTitle = TTF_OpenFont("time.ttf", 100);

	SDL_Rect rectTitle = {0,20, tailleFenetreW, 50};
	SDL_Rect rectStart = {tailleFenetreW/2 - buttonWidth/2, tailleFenetreH/2 - buttonHeight/2, buttonWidth, buttonHeight};
	SDL_Rect rectExit = {tailleFenetreW/2 - buttonWidth/2, tailleFenetreH/2 - buttonHeight/2 + 100, buttonWidth, buttonHeight};
	SDL_Color white = {255,255,255,255};
	SDL_Color lightBlue = {18,247,255,255};
	SDL_Color black = {0,0,0,255};
	Button buttonStart = CreateButton(rectStart,white, lightBlue, "Main menu", black, black);
	Button buttonExit = CreateButton(rectExit,white, lightBlue, "Exit", black, black);
	SetSelected(&buttonStart, 1);

	Button buttons[2] = {buttonStart, buttonExit};

	int index = 0;
	
	State currentState = GAME_OVER;
	
	while (currentState == GAME_OVER)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // fond noir
		SDL_RenderClear(renderer);

		// Boucle principale
		if (SDL_PollEvent(&event))
		{ // Scrute sans cesse les evenements et renvoie 1
			switch (event.type)
			{
				case SDL_QUIT: // Evenement fermeture de la fenetre
					currentState = EXIT;
					break;

				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							currentState = EXIT;
							break;

						case SDLK_z:
						case SDLK_UP:
							buttons[index].selected = 0;
							// index++;
							// if(index>=2){
							// 	index = 0;
							// }
							index = !index;
							buttons[index].selected = 1;
							break;
						
						case SDLK_s:
						case SDLK_DOWN:
							buttons[index].selected = 0;
							// index--;
							// if(index<0){
							// 	index = 1;
							// }
							index = !index;
							buttons[index].selected = 1;
							break;

						case SDLK_KP_ENTER:
						case SDLK_RETURN:
							if(index==0){currentState=MAIN;}
							else{currentState=EXIT;}
							break;
						
						default:
							break;
					}
					break;
			}
		}

		for (int i = 0; i < 2; i++)
		{
			displayButton(&buttons[i], renderer, font);
		}
		drawCenteredText(renderer, fontTitle, white, "GAME OVER", rectTitle);

		SDL_RenderPresent(renderer);
	}
	return currentState;
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
	
	int fin = 0;
	State currentState = MAIN;

	while (!fin)
	{
		switch (currentState)
		{
			case MAIN:
				currentState = MainMenu(renderer, event);
				break;

			case GAME_OVER:
				currentState = GameOverMenu(renderer, event);
				break;

			case GAME:
				currentState = game(player, renderer, keyboard, event, effects);
				break;

			case EXIT:
			default:
				fin = 1;
				break;
		}
	}
	
	destroyPlayer(player);
	destroyEffects(&effects);
	
	SDL_DestroyWindow(mafenetre);
	TTF_Quit();
	SDL_Quit();
	exit(0);
}
