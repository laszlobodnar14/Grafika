#include "camera.h"
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <math.h>
#include <stdbool.h>

#define DEG2RAD(x) ((x) * 3.14159265f / 180.0f)

typedef struct {
    float x, y, z;
    float rotationY;
} Chair;

#define NUM_CHAIRS 4
extern Chair chairs[];

bool collidesWithChair(float nextX, float nextZ) {
    const float chairRadius = 0.6f;
    const float cameraRadius = 0.3f;

    for (int i = 0; i < NUM_CHAIRS; ++i) {
        float dx = chairs[i].x - nextX;
        float dz = chairs[i].z - nextZ;
        float distSquared = dx * dx + dz * dz;
        float minDist = chairRadius + cameraRadius;

        if (distSquared < minDist * minDist) {
            return true;
        }
    }
    return false;
}

const float ROOM_MIN_X = -5.0f;
const float ROOM_MAX_X = 5.0f;
const float ROOM_MIN_Z = -5.0f;
const float ROOM_MAX_Z = 5.0f;

bool collidesWithWall(float nextX, float nextZ) {
    const float cameraRadius = 0.3f;

    if (nextX - cameraRadius < ROOM_MIN_X) return true;
    if (nextX + cameraRadius > ROOM_MAX_X) return true;
    if (nextZ - cameraRadius < ROOM_MIN_Z) return true;
    if (nextZ + cameraRadius > ROOM_MAX_Z) return true;

    return false;
}

void camera_init(Camera* cam) {
    cam->x = 0.0f;
    cam->y = 1.6f;
    cam->z = 0.0f;
    cam->yaw = 0.0f;
    cam->pitch = 0.0f;
    cam->speed = 3.0f;
    cam->sensitivity = 0.1f;
}

void camera_apply_transform(const Camera* cam) {
    glRotatef(-cam->pitch, 1.0f, 0.0f, 0.0f);
    glRotatef(-cam->yaw, 0.0f, 1.0f, 0.0f);
    glTranslatef(-cam->x, -cam->y, -cam->z);
}

void camera_handle_event(Camera* cam, SDL_Event* event) {
    if (event->type == SDL_MOUSEMOTION) {
        cam->yaw -= event->motion.xrel * cam->sensitivity;
        cam->pitch += event->motion.yrel * cam->sensitivity;

        if (cam->pitch > 89.0f) cam->pitch = 89.0f;
        if (cam->pitch < -89.0f) cam->pitch = -89.0f;
    }
}

void camera_update(Camera* cam, const Uint8* keyboard_state, float deltaTime) {
    float yawRad = DEG2RAD(cam->yaw);

    float forwardX = sinf(yawRad);
    float forwardZ = cosf(yawRad);

    float rightX = cosf(yawRad);
    float rightZ = -sinf(yawRad);

    float velocity = cam->speed * deltaTime;


    float nextX = cam->x;
    float nextZ = cam->z;

    if (keyboard_state[SDL_SCANCODE_S]) {
        nextX += forwardX * velocity;
        nextZ += forwardZ * velocity;
    }
    if (keyboard_state[SDL_SCANCODE_W]) {
        nextX -= forwardX * velocity;
        nextZ -= forwardZ * velocity;
    }
    if (keyboard_state[SDL_SCANCODE_A]) {
        nextX -= rightX * velocity;
        nextZ -= rightZ * velocity;
    }
    if (keyboard_state[SDL_SCANCODE_D]) {
        nextX += rightX * velocity;
        nextZ += rightZ * velocity;
    }

    if (!collidesWithChair(nextX, nextZ) && !collidesWithWall(nextX, nextZ)) {
        cam->x = nextX;
        cam->z = nextZ;
    }
}
