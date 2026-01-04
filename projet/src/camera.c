#include "camera.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


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

void CheckRays(Camera *camera, int raysOffestNumber, SDL_Rect walls[], int wallNumber, SDL_Rect items[], int itemCount, int screenW, int screenH, SDL_Renderer *renderer3D, SDL_Texture *itemTexture){
    float angleX, angleY = 0;
    int startOffset = 50;
    float startAngle = camera->radiant - DEG2RAD(camera->fov/2);

    Vector2 end;

    float colW = (float)screenW / camera->fov;
    float maxDist = startOffset * raysOffestNumber;
    float shade, disH, ca = 0;
    int lineH, lineOff, wallX = 0;
    int wallSize = 64;

    for(int r = 0; r < camera->fov; r++) {
        angleX = cosf(startAngle + DEG2RAD(1) * (r + 1));
        angleY = sinf(startAngle + DEG2RAD(1) * (r + 1));

        /* cast a ray and find nearest wall and nearest item along it */
        Vector2 rayFar = CreateVector(camera->position.x + angleX * maxDist, camera->position.y + angleY * maxDist);

        float nearestWallDist = -1.0f;
        Vector2 nearestWallHit = {0,0};
        for (int j = 0; j < wallNumber; j++) {
            Vector2 tmpHit;
            if (LineRectIntersect(camera->position, rayFar, &walls[j], &tmpHit)) {
                float d = Distance(camera->position, tmpHit);
                if (nearestWallDist < 0 || d < nearestWallDist) {
                    nearestWallDist = d;
                    nearestWallHit = tmpHit;
                }
            }
        }

        float nearestItemDist = -1.0f;
        Vector2 nearestItemHit = {0,0};
        int nearestItemIndex = -1;
        for (int k = 0; k < itemCount; k++) {
            Vector2 tmpHit;
            if (LineRectIntersect(camera->position, rayFar, &items[k], &tmpHit)) {
                float d = Distance(camera->position, tmpHit);
                if (nearestItemDist < 0 || d < nearestItemDist) {
                    nearestItemDist = d;
                    nearestItemHit = tmpHit;
                    nearestItemIndex = k;
                }
            }
        }

        /* Draw wall column first if exists */
        if (renderer3D != NULL && nearestWallDist > 0) {
            disH = nearestWallDist;
            ca = camera->radiant - (startAngle + DEG2RAD(1) * (r + 1));
            if (ca < 0) ca += 2 * M_PI;
            if (ca > 2 * M_PI) ca -= 2 * M_PI;
            // optional fisheye fix: disH = disH * cosf(ca);
            lineH = (wallSize * screenH) / disH;
            if (lineH > screenH) lineH = screenH;
            lineOff = (screenH / 2) - (lineH / 2);
            wallX = (int)(r * colW);
            shade = 1.0f - (disH / maxDist);
            if (shade < 0.1f) shade = 0.1f;
            if (shade > 1.0f) shade = 1.0f;
            SDL_SetRenderDrawColor(renderer3D, (int)(255 * shade), 0, 0, 255);
            for (int w = 0; w < 8; w++) {
                SDL_RenderDrawLine(renderer3D, wallX + w, lineOff - wallSize, wallX + w, lineOff + lineH - wallSize);
            }
        }

        /* Draw item overlay if item is closer than wall (or no wall) */
        if (renderer3D != NULL && nearestItemDist > 0) {
            if (nearestWallDist < 0 || nearestItemDist < nearestWallDist) {
                float itemDist = nearestItemDist;
                float itemShade = 1.0f - (itemDist / maxDist);
                if (itemShade < 0.1f) itemShade = 0.1f;
                if (itemShade > 1.0f) itemShade = 1.0f;

                int projH = (items[nearestItemIndex].h * screenH) / (int)itemDist;
                if (projH > screenH) projH = screenH;
                int projW = projH; // square sprite
                int projY = (screenH / 2) - (projH / 2);
                int projX = (int)(r * colW) - (projW / 2);

                if (itemTexture) {
                    SDL_Rect dst = { projX, projY, projW, projH };
                    SDL_SetTextureAlphaMod(itemTexture, (Uint8)(255 * itemShade));
                    SDL_RenderCopy(renderer3D, itemTexture, NULL, &dst);
                } else {
                    SDL_SetRenderDrawColor(renderer3D, (Uint8)(200 * itemShade), (Uint8)(120 * itemShade), 50, 255);
                    SDL_Rect cup = { projX, projY, projW, projH };
                    SDL_RenderFillRect(renderer3D, &cup);
                }
            }
        }
    }
}


