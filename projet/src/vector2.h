#ifndef VECTOR2_H
#define VECTOR2_H

#include <SDL.h>
#include <float.h>
#include <math.h>

typedef struct vector2{
    float x;
    float y;
} Vector2;

Vector2 createVectorZero();
Vector2 createVector(float x, float y);
int LineRectIntersect(
    Vector2 pos1, Vector2 pos2,
    SDL_Rect *rect,
    Vector2 *posOut
);

int LineLineIntersect(
    Vector2 a1, Vector2 a2,
    Vector2 b1, Vector2 b2,
    Vector2 *out
);

#endif