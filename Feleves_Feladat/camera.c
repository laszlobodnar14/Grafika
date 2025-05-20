
#include "camera.h"
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <math.h>

#define DEG2RAD(x) ((x) * 3.14159265f / 180.0f)

void camera_init(Camera* cam) {
    cam->x = 0.0f;
    cam->y = 1.6f;
    cam->z = 5.0f;
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
        cam->yaw += event->motion.xrel * cam->sensitivity;
        cam->pitch += event->motion.yrel * cam->sensitivity;


        if (cam->pitch > 89.0f) cam->pitch = 89.0f;
        if (cam->pitch < -89.0f) cam->pitch = -89.0f;
    }
}

void camera_update(Camera* cam, const Uint8* keyboard_state, float deltaTime) {

    float yawRad = DEG2RAD(cam->yaw);

    float forwardX = sinf(yawRad);
    float forwardZ = -cosf(yawRad);

    float rightX = cosf(yawRad);
    float rightZ = sinf(yawRad);

    float velocity = cam->speed * deltaTime;


    if (keyboard_state[SDL_SCANCODE_W]) {
        cam->x += forwardX * velocity;
        cam->z += forwardZ * velocity;
    }
    if (keyboard_state[SDL_SCANCODE_S]) {
        cam->x -= forwardX * velocity;
        cam->z -= forwardZ * velocity;
    }
    if (keyboard_state[SDL_SCANCODE_A]) {
        cam->x -= rightX * velocity;
        cam->z -= rightZ * velocity;
    }
    if (keyboard_state[SDL_SCANCODE_D]) {
        cam->x += rightX * velocity;
        cam->z += rightZ * velocity;
    }
}

