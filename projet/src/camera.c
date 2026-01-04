#include "camera.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int depth[60];

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


void CheckRaysGrid(Camera *camera, int raysOffsetNumber, int screenW, int screenH, SDL_Renderer *renderer3D){
    float startAngle = camera->radiant - DEG2RAD(camera->fov / 2);
    float colW = (float)screenW / camera->fov;
    int wallSize = 64;

    for (int r = 0; r < camera->fov; r++) {
        float rayAngle = startAngle + DEG2RAD(r);
        float rayX = cosf(rayAngle);
        float rayY = sinf(rayAngle);

        float dist = 0;
        int hit = 0;
        float hitX = 0, hitY = 0;

        for (int i = 1; i <= raysOffsetNumber; i++) {
            float stepDist = i * 20.0f; // pas du ray
            float px = camera->position.x + rayX * stepDist;
            float py = camera->position.y + rayY * stepDist;

            int gridX = (int)(px / wallSize);
            int gridY = (int)(py / wallSize);

            if (gridX < 0 || gridX >= MAP_W || gridY < 0 || gridY >= MAP_H_) break; // hors de la grille

            if (grid[gridY][gridX] == 1) { // collision mur
                hit = 1;
                hitX = px;
                hitY = py;
                dist = stepDist;
                break;
            }
        }

        if (hit && renderer3D) {
            // Correction du fisheye
            float ca = camera->radiant - rayAngle;
            if (ca < 0) ca += 2 * M_PI;
            if (ca > 2 * M_PI) ca -= 2 * M_PI;
            dist = dist * cosf(ca);

            int lineH = (int)((wallSize * screenH) / dist);
            if (lineH > screenH) lineH = screenH;

            int lineOff = (screenH / 2) - (lineH / 2);
            int wallX = (int)(r * colW);

            float shade = 1.0f - (dist / (raysOffsetNumber * wallSize));
            if (shade < 0.1f) shade = 0.1f;
            if (shade > 1.0f) shade = 1.0f;

            SDL_SetRenderDrawColor(renderer3D, (Uint8)(255 * shade), 0, 0, 255);

            for (int w = 0; w < 8; w++) {
                SDL_RenderDrawLine(renderer3D,
                                   wallX + w,
                                   lineOff,
                                   wallX + w,
                                   lineOff + lineH);
            }
        }
    }
}

void CheckRaysGridDDA(Camera *camera, int screenW, int screenH, SDL_Renderer *renderer3D) {
    float colW = (float)screenW / camera->fov;
    int wallSize = 64;

    float startAngle = camera->radiant - DEG2RAD(camera->fov / 2);

    for (int r = 0; r < camera->fov; r++) {
        float rayAngle = startAngle + DEG2RAD(r);
        float rayX = cosf(rayAngle);
        float rayY = sinf(rayAngle);

        // Position dans la grille
        int mapX = (int)(camera->position.x / wallSize);
        int mapY = (int)(camera->position.y / wallSize);

        // Distance au prochain bord vertical et horizontal
        float sideDistX, sideDistY;

        // Distance entre les lignes verticales et horizontales
        float deltaDistX = (rayX == 0) ? 1e30f : fabsf(wallSize / rayX);
        float deltaDistY = (rayY == 0) ? 1e30f : fabsf(wallSize / rayY);

        int stepX, stepY;
        int hit = 0;
        int side = 0; // 0 = vertical, 1 = horizontal

        // Calcul du step et du sideDist initial
        if (rayX < 0) {
            stepX = -1;
            sideDistX = (camera->position.x - mapX * wallSize) * deltaDistX / wallSize;
        } else {
            stepX = 1;
            sideDistX = ((mapX + 1) * wallSize - camera->position.x) * deltaDistX / wallSize;
        }

        if (rayY < 0) {
            stepY = -1;
            sideDistY = (camera->position.y - mapY * wallSize) * deltaDistY / wallSize;
        } else {
            stepY = 1;
            sideDistY = ((mapY + 1) * wallSize - camera->position.y) * deltaDistY / wallSize;
        }

        // DDA loop
        while (!hit) {
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }

            // Vérification des limites
            if (mapX < 0 || mapX >= MAP_W || mapY < 0 || mapY >= MAP_H_) break;

            if (grid[mapY][mapX] == 1) hit = 1; // collision mur
        }

        if (hit && renderer3D) {
            // Calcul de la distance exacte au mur
            float dist;
            if (side == 0) {
                dist = (mapX - camera->position.x / wallSize + (1 - stepX) / 2) * wallSize / rayX;
                SDL_SetRenderDrawColor(renderer3D, 40, 40, 40, 255);
            } else {
                dist = (mapY - camera->position.y / wallSize + (1 - stepY) / 2) * wallSize / rayY;
                SDL_SetRenderDrawColor(renderer3D, 20, 20, 20, 255);
            }

            // Correction du fish-eye
            float ca = camera->radiant - rayAngle;
            dist *= cosf(ca);

            depth[r] = dist;

            // Hauteur du mur
            int lineH = (int)((wallSize * screenH) / dist);
            if (lineH > screenH) lineH = screenH;

            int lineOff = (screenH / 2) - (lineH / 2);
            int wallX = (int)(r * colW);

            // float shade = 1.0f - fminf(dist / (MAP_W * wallSize), 1.0f);
            // if (shade < 0.1f) shade = 0.1f;


            for (int w = 0; w < MAP_W; w++) {
                SDL_RenderDrawLine(renderer3D,
                                   wallX + w,
                                   lineOff,
                                   wallX + w,
                                   lineOff + lineH);
            }
        }
    }
}

void drawItem(ItemVisualisation *item, Camera *camera, int screenW, int screenH, SDL_Renderer *renderer){
    // float colW = (float)screenW / camera->fov;
    // int wallSize = 64;
    // int mapX = (int)(camera->position.x / wallSize);
    // int mapY = (int)(camera->position.y / wallSize);
    
    // float dx = item->position.x - mapX;
    // float dy = item->position.y - mapY;

    // float itemAngle = atan2f(dy, dx);
    // float deltaAngle = itemAngle - camera->radiant;

    // while (deltaAngle > M_PI)  deltaAngle -= 2 * M_PI;
    // while (deltaAngle < -M_PI) deltaAngle += 2 * M_PI;
    
    
    // float halfFov = DEG2RAD(camera->fov) * 0.5f;
    
    // if (fabsf(deltaAngle) > halfFov) {
    //     return;
    // }
    
    // float screenXRatio = (deltaAngle + halfFov) / (2.0f * halfFov);
    // int itemScreenX = (int)(screenXRatio * screenW);
    
    // float dist = sqrtf(dx * dx + dy * dy);
    // float inshallahDist = dist * 10;
    
    // int r = (int) (deltaAngle+halfFov) * 180/ M_PI;
    // if(depth[r]<inshallahDist){
    //     return;
    // }
    // printf("%d, %.2f, %.2f\n", itemScreenX, screenXRatio, r*colW);

    // dist *= cosf(deltaAngle);

    // int itemSize = (int)((screenH) / dist) / 2;
    // int itemTop = (screenH / 2) - (itemSize / 2);

    // SDL_Rect rect;
    // rect.w = itemSize;
    // rect.h = itemSize;
    // rect.x = itemScreenX - (itemSize / 2);
    // rect.y = itemTop;

    // // printf("%d %d %d %d\n", rect.x, rect.y, rect.w, rect.h);
    // SDL_SetRenderDrawColor(renderer, 240, 200, 0, 255);
    // SDL_RenderFillRect(renderer, &rect);
}

// void drawItems(ItemVisualisation *items, int itemCount, SDL_Renderer *renderer){
//     for(int i = 0; i<itemCount; i++);
// }