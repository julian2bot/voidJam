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

// void drawMusicPlayer(SDL_Renderer *renderer, MusicPlayer playerUI)
// {
//     // Boîtier du poste horizontal
//     SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
//     SDL_Rect body = {580, SCREEN_HEIGHT-220, 250, 120};
//     SDL_RenderFillRect(renderer, &body);

//     // Volume knob à gauche
//     SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
//     int cx = playerUI.volumeKnob.x + playerUI.volumeKnob.w / 2;
//     int cy = playerUI.volumeKnob.y + playerUI.volumeKnob.h / 2;
//     int r = playerUI.volumeKnob.w / 2;
//     drawRing(renderer, cx, cy, 0, r);

//     // Indicateur de volume
//     float angle = playerUI.volumeAngle * M_PI / 180.0f;
//     int x2 = cx + cosf(angle) * (r-5);
//     int y2 = cy + sinf(angle) * (r-5);
//     SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
//     SDL_RenderDrawLine(renderer, cx, cy, x2, y2);

//     // Nom de la musique à droite
//     SDL_Color white = {255, 255, 255, 255};
//     drawText(renderer, NULL, &white, playerUI.currentTrack,
//              cx + r + 20, cy - 10, 14);

//     // Boutons sous le volume knob
//     int buttonY = cy + r + 10;
//     SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
//     playerUI.prevButton = (SDL_Rect){cx - r, buttonY, 20, 20};
//     playerUI.pauseButton = (SDL_Rect){cx - 10, buttonY, 20, 20};
//     playerUI.nextButton = (SDL_Rect){cx + r - 20, buttonY, 20, 20};
//     SDL_RenderFillRect(renderer, &playerUI.prevButton);
//     SDL_RenderFillRect(renderer, &playerUI.pauseButton);
//     SDL_RenderFillRect(renderer, &playerUI.nextButton);

//     // CD slot à droite du volume knob
//     playerUI.cdSlot = (SDL_Rect){cx + r + 10, cy + 20, 80, 30};
//     SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
//     SDL_RenderFillRect(renderer, &playerUI.cdSlot);

//     // Power button en haut à gauche du rectangle
//     playerUI.powerButton = (SDL_Rect){body.x + 10, body.y + 10, 30, 30};
//     SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
//     SDL_RenderFillRect(renderer, &playerUI.powerButton);
// }
// void drawMusicPlayer(SDL_Renderer *renderer, MusicPlayer playerUI)
// {
//     // Boîtier principal
//     SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
//     SDL_Rect body = {580, SCREEN_HEIGHT - 220, 300, 150};
//     SDL_RenderFillRect(renderer, &body);

//     // -------------------------------
//     // Volume knob à gauche
//     // -------------------------------
//     int knobCx = body.x + 40;
//     int knobCy = body.y + 50;
//     int knobR  = 25;
//     playerUI.volumeKnob = (SDL_Rect){knobCx - knobR, knobCy - knobR, knobR*2, knobR*2};

//     SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
//     drawRing(renderer, knobCx, knobCy, 0, knobR);

//     // Indicateur de volume
//     float angle = playerUI.volumeAngle * M_PI / 180.0f;
//     int x2 = knobCx + cosf(angle) * (knobR - 5);
//     int y2 = knobCy + sinf(angle) * (knobR - 5);
//     SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
//     SDL_RenderDrawLine(renderer, knobCx, knobCy, x2, y2);

//     // -------------------------------
//     // Nom de la musique à droite du volume
//     // -------------------------------
//     SDL_Color white = {255, 255, 255, 255};
//     int textX = knobCx + knobR + 20;
//     int textY = knobCy - 10;
//     drawText(renderer, NULL, &white, playerUI.currentTrack, textX, textY, 14);

//     // -------------------------------
//     // Boutons Prev / Pause / Next sous le volume knob
//     // -------------------------------
//     int buttonY = knobCy + knobR + 10;
//     int buttonSize = 25;
//     int spacing = 10;

//     playerUI.prevButton  = (SDL_Rect){knobCx - buttonSize - spacing, buttonY, buttonSize, buttonSize};
//     playerUI.pauseButton = (SDL_Rect){knobCx - buttonSize/2, buttonY, buttonSize, buttonSize};
//     playerUI.nextButton  = (SDL_Rect){knobCx + spacing + buttonSize/2, buttonY, buttonSize, buttonSize};

//     SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
//     SDL_RenderFillRect(renderer, &playerUI.prevButton);
//     SDL_RenderFillRect(renderer, &playerUI.pauseButton);
//     SDL_RenderFillRect(renderer, &playerUI.nextButton);

//     // -------------------------------
//     // CD slot tout en bas, 80% largeur
//     // -------------------------------
//     int cdWidth = body.w * 0.8;
//     int cdHeight = 30;
//     int cdX = body.x + (body.w - cdWidth) / 2;
//     int cdY = body.y + body.h - cdHeight - 10;
//     playerUI.cdSlot = (SDL_Rect){cdX, cdY, cdWidth, cdHeight};

//     SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
//     SDL_RenderFillRect(renderer, &playerUI.cdSlot);

//     // -------------------------------
//     // Power button en haut à gauche
//     // -------------------------------
//     playerUI.powerButton = (SDL_Rect){body.x + 10, body.y + 10, 30, 30};
//     SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
//     SDL_RenderFillRect(renderer, &playerUI.powerButton);
// }


// //  |-------------------------------|-------------------------------
// //  |   ____                          
// //  |  /    \    TITRE - TRUC MACHIN                     
// //  |  |     |                        
// //  |  \ ___/                         
// //  |             ==========================================                    
// //  |  <  ||  >                       
// //  |-------------------------------|-------------------------------



// void handleMusicPlayerClick(int mouseX, int mouseY, MusicPlayer playerUI)
// {
//     if(SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &playerUI.powerButton))
//     {
//         printf("power\n");
//     }
//     else if(SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &playerUI.cdSlot))
//     {
//         printf("insert CD\n");
//     }
//     else if(SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &playerUI.prevButton))
//     {
//         printf("previous track\n");
//     }
//     else if(SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &playerUI.pauseButton))
//     {
//         printf("pause\n");
//     }
//     else if(SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &playerUI.nextButton))
//     {
//         printf("next track\n");
//     }
//     else if(SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &playerUI.volumeKnob))
//     {
//         // rotation du volume, ici juste un exemple
//         playerUI.volumeAngle += 15.0f;
//         if(playerUI.volumeAngle > 360) playerUI.volumeAngle -= 360;
//         printf("volume rotated: %.1f°\n", playerUI.volumeAngle);
//     }
// }
void drawMusicPlayer(SDL_Renderer *renderer, MusicPlayer *playerUI)
{
    // Boîtier principal
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_Rect body = {580, SCREEN_HEIGHT - 220, 300, 150};
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

    // // -------------------------------
    // // Boutons Prev / Pause / Next sous le volume knob
    // // -------------------------------
    // int buttonY = knobCy + knobR + 10;
    // int buttonSize = 25;
    // int spacing = 10;

    // playerUI->prevButton  = (SDL_Rect){knobCx - buttonSize - spacing, buttonY, buttonSize, buttonSize};
    // playerUI->pauseButton = (SDL_Rect){knobCx - buttonSize/2, buttonY, buttonSize, buttonSize};
    // playerUI->nextButton  = (SDL_Rect){knobCx + spacing + buttonSize/2, buttonY, buttonSize, buttonSize};

    // SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    // SDL_RenderFillRect(renderer, &playerUI->prevButton);
    // SDL_RenderFillRect(renderer, &playerUI->pauseButton);
    // SDL_RenderFillRect(renderer, &playerUI->nextButton);

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

    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &playerUI->prevButton);
    SDL_RenderFillRect(renderer, &playerUI->pauseButton);
    SDL_RenderFillRect(renderer, &playerUI->nextButton);

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
