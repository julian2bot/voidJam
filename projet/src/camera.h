#ifndef CAMERA_H
#define CAMERA_H

#include <SDL.h>
#include "player.h"
#include "vector2.h"

typedef struct camera{
    Vector2 position;
    float radiant;
    float fov;
    float planeX; // vecteur du camera plane (perpendiculaire à dir)
    float planeY;
} Camera;

Camera CreateCamera(Vector2 position, float rotation, float fov);
void CalculatePlane(Camera *camera);
void UpdatePosition(Camera *camera, Vector2 vector2);
void UpdateRotation(Camera *camera, float rotation);
void CheckRays(Camera *camera, int raysNumber, int raysOffestNumber, SDL_Rect walls[], int wallNumber, SDL_Renderer *renderer);

void UpdateCameraPlayer(Camera *camera, Player *player);

#endif