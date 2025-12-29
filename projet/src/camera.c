#include "camera.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define DEG2RAD(x) ((x) * M_PI / 180.0f)

Camera CreateCamera(Vector2 position, float rotation, float fov){
    Camera camera = {position, DEG2RAD(rotation), fov, 0,0};
    UpdateRotation(&camera, rotation);
    CalculatePlane(&camera);
    return camera;
}

void CalculatePlane(Camera *camera){
    float fovRad = DEG2RAD(camera->fov);
    float halfPlaneSize = tanf(fovRad / 2);
    camera->planeX = - cosf(camera->radiant) * halfPlaneSize;
    camera->planeY =  sinf(camera->radiant) * halfPlaneSize;
};

void UpdatePosition(Camera *camera, Vector2 position){
    camera->position = position;
}

void UpdateRotation(Camera *camera, float rotation){
    camera->radiant = DEG2RAD(rotation);
}

void UpdateCameraPlayer(Camera *camera, Player *player){
    float rad = DEG2RAD(player->angle);
    
    int centerX = player->position.x + player->position.w / 2;
    int centerY = player->position.y + player->position.h / 2;

    Vector2 newPos = {centerX, centerY};
    UpdatePosition(camera, newPos);
    UpdateRotation(camera, player->angle);
}

void CheckRays(Camera *camera, int raysOffestNumber, SDL_Rect walls[], int wallNumber, int screenW, int screenH, SDL_Renderer *renderer2D, SDL_Renderer *renderer3D){
    float angleX, angleY = 0;
    int startOffset = 50;
    float startAngle = camera->radiant - DEG2RAD(camera->fov/2);

    Vector2 end;
    Vector2 hit;

    int hasHit = 0;
    float colW = (float)screenW / camera->fov;
    int thickness = (int)ceilf(colW);
    float maxDist = startOffset * raysOffestNumber;

    for(int r = 0; r<camera->fov; r++){
        angleX = cosf(startAngle + DEG2RAD(1)*(r+1));
        angleY = sinf(startAngle + DEG2RAD(1)*(r+1));

        for (int i = 0; i < raysOffestNumber; i++)
        {
            end = CreateVector(camera->position.x + angleX * (startOffset * (i+1)),camera->position.y + angleY * (startOffset * (i+1)));

            for (int i = 0; i < wallNumber; i++)
            {
                hasHit = LineRectIntersect(camera->position, end, &walls[i], &hit);
                if (hasHit)
                {
                    end.x = hit.x;
                    end.y = hit.y;
                    break;
                }
            }
            if (hasHit)
            {
                /* ---------- DISTANCE ---------- */
                float disH = Distance(camera->position, end);

                /* ---------- FIX FISHEYE ---------- */
                float ca = camera->radiant - (startAngle + DEG2RAD(1) * (r + 1));

                /* normalisation angle */
                if (ca < 0) ca += 2 * M_PI;
                if (ca > 2 * M_PI) ca -= 2 * M_PI;

                // disH = disH * cosf(ca);

                /* ---------- HAUTEUR DU MUR ---------- */
                int lineH = (64*screenH) / disH;
                if (lineH > screenH) lineH = screenH;

                /* ---------- OFFSET VERTICAL ---------- */
                int lineOff = (screenH / 2) - (lineH / 2);

                /* ---------- POSITION X DU RAYON ---------- */
                int wallX = (int)(r * colW);

                /* ---------- COULEUR ---------- */
                float shade = 1.0f - (disH / maxDist);

                if (shade < 0.1f) shade = 0.1f;
                if (shade > 1.0f) shade = 1.0f;

                Uint8 color = (Uint8)(255 * shade);
                printf("color %d\n", color);
                SDL_SetRenderDrawColor(renderer3D, color, 0, 0, 255);

                /* ---------- ÉPAISSEUR (équivalent glLineWidth(8)) ---------- */
                for (int w = 0; w < 8; w++)
                {
                    SDL_RenderDrawLine(
                        renderer3D,
                        wallX + w,
                        lineOff,
                        wallX + w,
                        lineOff + lineH
                    );
                }
                break;
            }
        }

        if(renderer2D!=NULL){
            if (hasHit){
                SDL_SetRenderDrawColor(renderer2D, 255, 0, 0, 255);
            }
            else{
                SDL_SetRenderDrawColor(renderer2D, 0, 255, 0, 255);
            }
            SDL_RenderDrawLine(renderer2D, camera->position.x, camera->position.y, end.x, end.y);
        }
    }
}


