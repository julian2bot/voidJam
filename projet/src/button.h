#ifndef BUTTON_H
#define BUTTON_H

#include "gestionSDL.h"

typedef struct Button{
    SDL_Rect rect;
    SDL_Color baseColor;
    SDL_Color selectedColor;
    char* text;
    SDL_Color baseTextColor;
    SDL_Color selectedTextColor;
    uint8_t selected;
}Button;

void SetSelected(Button* button, uint8_t value);
Button CreateButton(SDL_Rect rect, SDL_Color baseColor, SDL_Color selectedColor, char* text, SDL_Color baseTextColor, SDL_Color selectedTextColor);
void displayButton(Button* button, SDL_Renderer * renderer, TTF_Font *font);

#endif