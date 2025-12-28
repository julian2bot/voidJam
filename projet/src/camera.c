#include "camera.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define DEG2RAD(x) ((x) * M_PI / 180.0f)

Camera createCamera(float posX, float posY, float rotation, float fov){
    Camera camera = {posX, posY, 0, 0, fov, 0,0};
    updateRotation(&camera, rotation);
    calculatePlane(&camera);
    return camera;
}

// Camera createCameraDefault(float posX, float posY, float rotation, float fov){
//     return createCamera(posX, posY, rotation, fov, 1);
// }

// void updateDistancePlane(Camera *camera, float distancePlane){
//     camera->distancePlane = distancePlane;
//     calculatePlane(camera);
// }

void calculatePlane(Camera *camera){
    float fovRad = DEG2RAD(camera->fov);
    // float halfPlaneSize = camera->distancePlane * tanf(fovRad / 2);
    float halfPlaneSize = tanf(fovRad / 2);
    camera->planeX = -camera->dirY * halfPlaneSize;
    camera->planeY =  camera->dirX * halfPlaneSize;
};

void updatePosition(Camera *camera, float posX, float posY){
    camera->posX = posX;
    camera->posY = posY;
}

void updateRotation(Camera *camera, float rotation){
    float r = DEG2RAD(rotation);
    camera->dirX = cosf(r);
    camera->dirY = sinf(r);
}

