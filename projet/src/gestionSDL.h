#ifndef GESTIONSDL_H
#define GESTIONSDL_H
#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <math.h>
#define DEG2RAD(x) ((x) * M_PI / 180.0f)

SDL_Texture *getTextureFromImage(const char *nomPic, SDL_Renderer *renderer);
void destroy_textures(unsigned int nb_textures, ...);
void drawText(SDL_Renderer *renderer, TTF_Font *font, SDL_Color *color, const char *text, int x, int y, int size);
void drawRing(SDL_Renderer *r, int cx, int cy, int r1, int r2);
void drawCenteredText(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, const char *text, SDL_Rect container);

#endif