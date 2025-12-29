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
void CheckRays(Camera *camera, int raysOffestNumber, SDL_Rect walls[], int wallNumber, int screenW, int screenH, SDL_Renderer *renderer2D, SDL_Renderer *renderer3D);
void UpdateCameraPlayer(Camera *camera, Player *player);

#endif