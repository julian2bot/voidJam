#ifndef CAMERA_H
#define CAMERA_H

typedef struct camera{
    float posX;
    float posY;
    float dirX; // Cos(angle)
    float dirY; // Sin(angle)
    float fov;
    float planeX; // vecteur du camera plane (perpendiculaire à dir)
    float planeY;
} Camera;

Camera createCamera(float posX, float posY, float rotation, float fov);
// Camera createCameraDefault(float posX, float posY, float rotation, float fov);
void calculatePlane(Camera *camera);
// void updateDistancePlane(Camera *camera, float distancePlane);
void updatePosition(Camera *camera, float posX, float posY);
void updateRotation(Camera *camera, float rotation);

#endif