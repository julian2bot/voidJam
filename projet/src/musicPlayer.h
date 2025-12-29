#ifndef musicPLAYER_H
#define musicPLAYER_H

#include <stdio.h>
#include <SDL.h>
#include "gestionSDL.h"
#include <math.h>
#define SCREEN_HEIGHT 600
#define SCREEN_WIDTH 1000

typedef struct MusicPlayer
{
    SDL_Rect powerButton;
    SDL_Rect volumeKnob;
    SDL_Rect cdSlot;
    SDL_Rect prevButton;
    SDL_Rect pauseButton;
    SDL_Rect nextButton;
    char currentTrack[64];
    float volumeAngle; // rotation pour volume knob
    int showPlaylist; // 0 = hidden, 1 = visible
} MusicPlayer;

MusicPlayer initMusicPlayer();
void drawMusicPlayer(SDL_Renderer *renderer, MusicPlayer* playerUI);
void handleMusicPlayerClick(int mouseX, int mouseY, MusicPlayer* playerUI);
void drawPlaylistOverlay(SDL_Renderer *renderer, MusicPlayer *playerUI);
void drawPauseIcon(SDL_Renderer *renderer, SDL_Rect r);
void drawNextIcon(SDL_Renderer *renderer, SDL_Rect r);
void drawPrevIcon(SDL_Renderer *renderer, SDL_Rect r);


#endif