#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include "camera.h"
#include "texture.h"
#include "room.h"

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("3D Szoba",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          800, 600,
                                          SDL_WINDOW_OPENGL);
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        fprintf(stderr, "SDL_GL_CreateContext error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat lightPos[] = {0.0f, 5.0f, 5.0f, 1.0f};
    GLfloat lightColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);

    Camera cam;
    camera_init(&cam);


    GLuint floorTexture = loadTexture("textures/floor.png");
    if (floorTexture == 0) {
        fprintf(stderr, "Nem sikerült betölteni a floor.bmp textúrát.\n");
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    GLuint wallTexture = loadTexture("textures/wall.png");
    if (wallTexture == 0) {
        fprintf(stderr, "Nem sikerült betölteni a wall.bmp textúrát.\n");
        glDeleteTextures(1, &floorTexture);
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }


    GLuint clockTexture = loadTexture("textures/ora.bmp");
    if (clockTexture == 0) {
        fprintf(stderr, "Nem sikerült betölteni az ora.bmp textúrát.\n");
        glDeleteTextures(1, &floorTexture);
        glDeleteTextures(1, &wallTexture);
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int running = 1;
    Uint32 lastTime = SDL_GetTicks();

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
            camera_handle_event(&cam, &event);
        }

        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
        camera_update(&cam, keyboardState, deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(70.0, 800.0 / 600.0, 0.1, 100.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();


        camera_apply_transform(&cam);


        drawRoom(floorTexture, wallTexture);


        glBindTexture(GL_TEXTURE_2D, clockTexture);
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex3f(-0.5f, 0.0f, -2.0f);
            glTexCoord2f(1, 0); glVertex3f(0.5f, 0.0f, -2.0f);
            glTexCoord2f(1, 1); glVertex3f(0.5f, 1.0f, -2.0f);
            glTexCoord2f(0, 1); glVertex3f(-0.5f, 1.0f, -2.0f);
        glEnd();

        SDL_GL_SwapWindow(window);
        SDL_Delay(10);
    }

    glDeleteTextures(1, &clockTexture);
    glDeleteTextures(1, &floorTexture);
    glDeleteTextures(1, &wallTexture);

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
