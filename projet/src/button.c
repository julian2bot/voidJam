#include "button.h"

void SetSelected(Button* button, uint8_t value){
    button->selected = value;
}

Button CreateButton(SDL_Rect rect, SDL_Color baseColor, SDL_Color selectedColor, char* text, SDL_Color baseTextColor, SDL_Color selectedTextColor){
    Button button = {rect, baseColor, selectedColor, text, baseTextColor, selectedTextColor,0};
    return button;
}

void displayButton(Button* button, SDL_Renderer *renderer, TTF_Font *font){
    SDL_Color color = (button->selected) ? button->selectedColor : button->baseColor;
    SDL_Color colorText = (button->selected) ? button->selectedTextColor : button->baseTextColor;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &(button->rect));
    drawCenteredText(renderer, font, colorText, button->text, button->rect);
}