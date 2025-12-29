#include "vector2.h"

Vector2 CreateVectorZero(){
    return CreateVector(0,0);
}

Vector2 CreateVector(float x, float y){
    Vector2 vector2 = {x,y};
    return vector2;
}

int LineLineIntersect(Vector2 a1, Vector2 a2, Vector2 b1, Vector2 b2, Vector2 *out)
{
    float den =
        (a1.x - a2.x) * (b1.y - b2.y) -
        (a1.y - a2.y) * (b1.x - b2.x);

    if (den == 0.0f)
        return 0; // parallèles

    float t =
        ((a1.x - b1.x) * (b1.y - b2.y) -
         (a1.y - b1.y) * (b1.x - b2.x)) / den;

    float u =
        -((a1.x - a2.x) * (a1.y - b1.y) -
          (a1.y - a2.y) * (a1.x - b1.x)) / den;

    if (t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f)
    {
        if (out)
        {
            out->x = a1.x + t * (a2.x - a1.x);
            out->y = a1.y + t * (a2.y - a1.y);
        }
        return 1;
    }
    return 0;
}

int LineRectIntersect(Vector2 pos1, Vector2 pos2, SDL_Rect *rect, Vector2 *posOut)
{
    Vector2 hit;
    float closestDist = FLT_MAX;
    int collided = 0;

    Vector2 r1 = { rect->x, rect->y };
    Vector2 r2 = { rect->x + rect->w, rect->y };
    Vector2 r3 = { rect->x + rect->w, rect->y + rect->h };
    Vector2 r4 = { rect->x, rect->y + rect->h };

    Vector2 tmp;

    // Haut
    if (LineLineIntersect(pos1, pos2, r1, r2, &tmp))
    {
        float d = hypotf(tmp.x - pos1.x, tmp.y - pos1.y);
        if (d < closestDist) { closestDist = d; hit = tmp; collided = 1; }
    }

    // Droite
    if (LineLineIntersect(pos1, pos2, r2, r3, &tmp))
    {
        float d = hypotf(tmp.x - pos1.x, tmp.y - pos1.y);
        if (d < closestDist) { closestDist = d; hit = tmp; collided = 1; }
    }

    // Bas
    if (LineLineIntersect(pos1, pos2, r3, r4, &tmp))
    {
        float d = hypotf(tmp.x - pos1.x, tmp.y - pos1.y);
        if (d < closestDist) { closestDist = d; hit = tmp; collided = 1; }
    }

    // Gauche
    if (LineLineIntersect(pos1, pos2, r4, r1, &tmp))
    {
        float d = hypotf(tmp.x - pos1.x, tmp.y - pos1.y);
        if (d < closestDist) { closestDist = d; hit = tmp; collided = 1; }
    }

    if (collided && posOut)
        *posOut = hit;

    return collided;
}

float Distance(Vector2 a, Vector2 b)
{
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    return sqrtf(dx * dx + dy * dy);
}
