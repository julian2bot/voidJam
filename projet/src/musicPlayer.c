#include "musicPlayer.h"
#include "audio.h"
#include <string.h>

MusicPlayer initMusicPlayer()
{
    
    MusicPlayer playerUI;
    playerUI.powerButton = (SDL_Rect){600, SCREEN_HEIGHT-200, 30, 30};
    playerUI.volumeKnob  = (SDL_Rect){650, SCREEN_HEIGHT-200, 40, 40};
    playerUI.cdSlot      = (SDL_Rect){610, SCREEN_HEIGHT-150, 80, 50};
    playerUI.prevButton  = (SDL_Rect){610, SCREEN_HEIGHT-90, 20, 20};
    playerUI.pauseButton = (SDL_Rect){640, SCREEN_HEIGHT-90, 20, 20};
    playerUI.nextButton  = (SDL_Rect){670, SCREEN_HEIGHT-90, 20, 20};

    const char *title = audio_get_current_title();
    if (title) strncpy(playerUI.currentTrack, title, sizeof(playerUI.currentTrack)-1);
    else strcpy(playerUI.currentTrack, "(no track)");
    playerUI.currentTrack[sizeof(playerUI.currentTrack)-1] = '\0';
    playerUI.volumeAngle = 0.0f;
    playerUI.showPlaylist = 0;
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
    // keep title in sync
    const char *title = audio_get_current_title();
    if (title) {
        strncpy(playerUI->currentTrack, title, sizeof(playerUI->currentTrack)-1);
        playerUI->currentTrack[sizeof(playerUI->currentTrack)-1] = '\0';
    }
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
        // printf("Power toggled!\n");
        audio_play(0);
    }
    else if(SDL_PointInRect(&p, &playerUI->cdSlot))
    {
        // toggle playlist overlay
        playerUI->showPlaylist = !playerUI->showPlaylist;
        printf("Playlist toggled: %d\n", playerUI->showPlaylist);
        return;
    }
    else if(SDL_PointInRect(&p, &playerUI->prevButton))
    {
        audio_prev();
        const char *t = audio_get_current_title(); if (t) strncpy(playerUI->currentTrack, t, sizeof(playerUI->currentTrack)-1);
    }
    else if(SDL_PointInRect(&p, &playerUI->pauseButton))
    {
        audio_pause_toggle();
    }
    else if(SDL_PointInRect(&p, &playerUI->nextButton))
    {
        audio_next();
        const char *t = audio_get_current_title(); if (t) strncpy(playerUI->currentTrack, t, sizeof(playerUI->currentTrack)-1);
    }
    else if(SDL_PointInRect(&p, &playerUI->volumeKnob))
    {
        // exemple de rotation simple
        playerUI->volumeAngle += 15.0f;
        if(playerUI->volumeAngle >= 360) playerUI->volumeAngle -= 360;
        // map angle to volume 0..1
        float vol = (playerUI->volumeAngle) / 360.0f;
        audio_set_volume(vol);
        printf("Volume rotated: %.1f° -> %.2f\n", playerUI->volumeAngle, vol);
    }

    // If playlist visible, check clicks on playlist entries
    if (playerUI->showPlaylist)
    {
        int count = audio_get_track_count();
        if (count > 0)
        {
            // playlist area (mirror drawPlaylist): fixed for simplicity
            SDL_Rect listRect = {450, 80, 300, 24 * count + 20};
            if (SDL_PointInRect(&p, &listRect))
            {
                int relY = mouseY - listRect.y - 10;
                int idx = relY / 24;
                if (idx >= 0 && idx < count)
                {
                    audio_select(idx);
                    const char *t = audio_get_current_title(); if (t) strncpy(playerUI->currentTrack, t, sizeof(playerUI->currentTrack)-1);
                    playerUI->showPlaylist = 0; // close
                    printf("Selected track %d\n", idx);
                }
            }
        }
    }
}

// Draw the playlist overlay (called from drawCockPit after drawMusicPlayer)
void drawPlaylistOverlay(SDL_Renderer *renderer, MusicPlayer *playerUI)
{
    if (!playerUI->showPlaylist) return;
    int count = audio_get_track_count();
    if (count == 0) return;

    SDL_Rect listRect = {450, 80, 300, 24 * count + 20};
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 230);
    SDL_RenderFillRect(renderer, &listRect);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderDrawRect(renderer, &listRect);

    SDL_Color white = {255,255,255,255};
    for (int i = 0; i < count; i++) {
        const char *t = audio_get_title_at(i);
        SDL_Rect item = { listRect.x + 10, listRect.y + 10 + i*24, listRect.w - 20, 20 };
        // highlight current
        const char *cur = audio_get_current_title();
        if (cur && t && strcmp(cur, t) == 0) {
            SDL_SetRenderDrawColor(renderer, 60, 60, 120, 255);
            SDL_RenderFillRect(renderer, &item);
        }
        drawText(renderer, NULL, &white, t ? t : "(untitled)", item.x+4, item.y+2, 14);
    }
}
