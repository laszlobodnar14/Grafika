#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>

typedef struct {
    float x, y, z;
    float yaw;
    float pitch;
    float speed;
    float sensitivity;
} Camera;

void camera_init(Camera* cam);
void camera_apply_transform(const Camera* cam);
void camera_handle_event(Camera* cam, SDL_Event* event);
void camera_update(Camera* cam, const Uint8* keyboard_state, float deltaTime);


#endif
