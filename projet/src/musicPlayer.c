#include "musicPlayer.h"

MusicPlayer initMusicPlayer()
{
    
    MusicPlayer playerUI;
    playerUI.powerButton = (SDL_Rect){600, SCREEN_HEIGHT-200, 30, 30};
    playerUI.volumeKnob  = (SDL_Rect){650, SCREEN_HEIGHT-200, 40, 40};
    playerUI.cdSlot      = (SDL_Rect){610, SCREEN_HEIGHT-150, 80, 50};
    playerUI.prevButton  = (SDL_Rect){610, SCREEN_HEIGHT-90, 20, 20};
    playerUI.pauseButton = (SDL_Rect){640, SCREEN_HEIGHT-90, 20, 20};
    playerUI.nextButton  = (SDL_Rect){670, SCREEN_HEIGHT-90, 20, 20};

    strcpy(playerUI.currentTrack, "Track 01 - Epic Song");
    playerUI.volumeAngle = 0.0f;
    return playerUI;
}


//  |-------------------------------|-------------------------------
//  |   ____
//  |  /    \    TITRE - TRUC MACHIN
//  |  |     |
//  |  \ ___/
//  |             ==========================================
//  |  <  ||  >
//  |-------------------------------|-------------------------------

void drawMusicPlayer(SDL_Renderer *renderer, MusicPlayer *playerUI)
{
    // Boîtier principal
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_Rect body = {430, SCREEN_HEIGHT - 165, 300, 150};
    SDL_RenderFillRect(renderer, &body);

    // -------------------------------
    // Volume knob à gauche
    // -------------------------------
    int knobCx = body.x + 40;
    int knobCy = body.y + 50;
    int knobR  = 25;
    playerUI->volumeKnob = (SDL_Rect){knobCx - knobR, knobCy - knobR, knobR*2, knobR*2};

    SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
    drawRing(renderer, knobCx, knobCy, 0, knobR);

    // Indicateur de volume
    float angle = playerUI->volumeAngle * M_PI / 180.0f;
    int x2 = knobCx + cosf(angle) * (knobR - 5);
    int y2 = knobCy + sinf(angle) * (knobR - 5);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawLine(renderer, knobCx, knobCy, x2, y2);

    // -------------------------------
    // Nom de la musique à droite du volume
    // -------------------------------
    SDL_Color white = {255, 255, 255, 255};
    drawText(renderer, NULL, &white, playerUI->currentTrack, knobCx + knobR + 20, knobCy - 10, 14);

    // -------------------------------
    // Boutons Prev / Pause / Next sous le volume knob
    // -------------------------------
    int buttonY = knobCy + knobR + 10;
    int buttonSize = 25;
    int spacing = 10;

    // position du bouton central (pause)
    int pauseX = knobCx - buttonSize/2;
    pauseX += 20;
    // boutons autour du central
    playerUI->prevButton  = (SDL_Rect){pauseX - buttonSize - spacing, buttonY, buttonSize, buttonSize};
    playerUI->pauseButton = (SDL_Rect){pauseX, buttonY, buttonSize, buttonSize};
    playerUI->nextButton  = (SDL_Rect){pauseX + buttonSize + spacing, buttonY, buttonSize, buttonSize};

    // SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    // SDL_RenderFillRect(renderer, &playerUI->prevButton);
    // SDL_RenderFillRect(renderer, &playerUI->pauseButton);
    // SDL_RenderFillRect(renderer, &playerUI->nextButton);

    // Dessin des boutons
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &playerUI->prevButton);
    SDL_RenderFillRect(renderer, &playerUI->pauseButton);
    SDL_RenderFillRect(renderer, &playerUI->nextButton);

    // Icônes (couleur foncée)
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);

    drawPrevIcon(renderer, playerUI->prevButton);
    drawPauseIcon(renderer, playerUI->pauseButton);
    drawNextIcon(renderer, playerUI->nextButton);


    // -------------------------------
    // CD slot tout en bas, 80% largeur
    // -------------------------------
    int cdWidth = body.w * 0.8;
    int cdHeight = 30;
    int cdX = body.x + (body.w - cdWidth) / 2;
    int cdY = body.y + body.h - cdHeight - 10;
    playerUI->cdSlot = (SDL_Rect){cdX, cdY, cdWidth, cdHeight};

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &playerUI->cdSlot);

    // -------------------------------
    // Power button en haut à gauche
    // -------------------------------
    playerUI->powerButton = (SDL_Rect){body.x + 10, body.y + 10, 30, 30};
    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
    SDL_RenderFillRect(renderer, &playerUI->powerButton);
}

// -------------------------------
// Gestion des clics interactifs
// -------------------------------
void handleMusicPlayerClick(int mouseX, int mouseY, MusicPlayer *playerUI)
{
    SDL_Point p = {mouseX, mouseY};

    if(SDL_PointInRect(&p, &playerUI->powerButton))
    {
        printf("Power toggled!\n");
    }
    else if(SDL_PointInRect(&p, &playerUI->cdSlot))
    {
        printf("Insert CD\n");
    }
    else if(SDL_PointInRect(&p, &playerUI->prevButton))
    {
        printf("Previous track\n");
    }
    else if(SDL_PointInRect(&p, &playerUI->pauseButton))
    {
        printf("Pause\n");
    }
    else if(SDL_PointInRect(&p, &playerUI->nextButton))
    {
        printf("Next track\n");
    }
    else if(SDL_PointInRect(&p, &playerUI->volumeKnob))
    {
        // exemple de rotation simple
        playerUI->volumeAngle += 15.0f;
        if(playerUI->volumeAngle >= 360) playerUI->volumeAngle -= 360;
        printf("Volume rotated: %.1f°\n", playerUI->volumeAngle);
    }
}
