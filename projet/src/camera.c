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

void CheckRays(Camera *camera, int raysNumber, int raysOffestNumber, SDL_Rect walls[], int wallNumber, SDL_Renderer *renderer){
    if(renderer!=NULL){
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    }

    float endX, endY = 0;
    float angleX, angleY = 0;
    int startOffset = 50;

    float startAngle = camera->radiant - DEG2RAD(camera->fov/2);
    float stepAngle = DEG2RAD(camera->fov/raysNumber);

    Vector2 end;
    Vector2 hit;
    int hasHit = 0;

    for(int r = 0; r<raysNumber; r++){
        angleX = cosf(startAngle + stepAngle*(r+1));
        angleY = sinf(startAngle + stepAngle*(r+1));

        for (int i = 0; i < raysOffestNumber; i++)
        {
            endX = camera->position.x + angleX * (startOffset * (i+1));
            endY = camera->position.y + angleY * (startOffset * (i+1));
            end = createVector(endX,endY);

            for (int i = 0; i < wallNumber; i++)
            {
                hasHit = LineRectIntersect(camera->position, end, &walls[i], &hit);
                if (hasHit)
                {
                    endX = hit.x;
                    endY = hit.y;
                    break;
                }
            }
            if (hasHit)
            {
                break;
            }
        }

        if(renderer!=NULL){
            SDL_RenderDrawLine(renderer, camera->position.x, camera->position.y, endX, endY);
        }
    }
}


