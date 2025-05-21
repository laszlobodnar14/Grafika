#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdbool.h>
#include "camera.h"
#include "texture.h"
#include "room.h"
#include "model.h"


typedef struct {
    float x, y, z;
    float rotationY;
} Chair;

#define NUM_CHAIRS 4

int selectedChair = -1;
Chair chairs[NUM_CHAIRS];

GLuint helpTexture = 0;
bool showHelp = false;

void reset_chairs() {
    chairs[0] = (Chair){1.5f, 0.0f, 1.0f, 0.0f};
    chairs[1] = (Chair){1.5f, 0.0f, 3.0f, 180.0f};
    chairs[2] = (Chair){0.0f, 0.0f, 2.0f, 90.0f};
    chairs[3] = (Chair){4.0f, 0.0f, 2.5f, -90.0f};
}

void renderHelpOverlay(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, helpTexture);

    glColor4f(1, 1, 1, 1);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex2f(0, 0);
        glTexCoord2f(1, 1); glVertex2f(width, 0);
        glTexCoord2f(1, 0); glVertex2f(width, height);
        glTexCoord2f(0, 0); glVertex2f(0, height);
    glEnd();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

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

    SDL_ShowCursor(SDL_DISABLE);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat lightPos[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    GLfloat matAmbient[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat matDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat matSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat matShininess[] = {50.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    float brightness = 1.0f;

    Camera cam;
    camera_init(&cam);

    reset_chairs();

    GLuint floorTexture = loadTexture("textures/floor.png");
    GLuint wallTexture = loadTexture("textures/wall.png");
    GLuint clockTexture = loadTexture("textures/clock1.png");
    GLuint szekTexture = loadTexture("models/Chair_Basecolor.jpg");
    GLuint tableTexture = loadTexture("models/Chair_Basecolor.jpg");
    helpTexture = loadTexture("textures/help.jpg");

    if (!floorTexture || !wallTexture || !clockTexture || !szekTexture || !tableTexture || !helpTexture) {
        fprintf(stderr, "Nem sikerült betölteni az összes textúrát.\n");
        return 1;
    }

    Model clockModel = loadOBJ("models/objClock.obj");
    Model szekModel = loadOBJ("models/szek.obj");
    Model tableModel = loadOBJ("models/Table.obj");

    if (clockModel.vertexCount == 0 || szekModel.vertexCount == 0 || tableModel.vertexCount == 0) {
        fprintf(stderr, "Nem sikerült betölteni az összes modellt.\n");
        return 1;
    }

    int running = 1;
    Uint32 lastTime = SDL_GetTicks();

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
            camera_handle_event(&cam, &event);

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_o:
                        brightness += 0.1f;
                        if (brightness > 2.0f) brightness = 2.0f;
                        break;
                    case SDLK_p:
                        brightness -= 0.1f;
                        if (brightness < 0.0f) brightness = 0.0f;
                        break;
                    case SDLK_1:
                    case SDLK_2:
                    case SDLK_3:
                    case SDLK_4:
                        selectedChair = event.key.keysym.sym - SDLK_1;
                        break;
                    case SDLK_q:
                        selectedChair = -1;
                        break;
                        case SDLK_ESCAPE:
                        running = 0;
                        break;
                    case SDLK_F1:
                        showHelp = true;
                        break;
                    case SDLK_F2:
                        showHelp = false;
                        break;
                    default:
                        break;
                }
            }
        }

        const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
        camera_update(&cam, keyboardState, (SDL_GetTicks() - lastTime) / 1000.0f);
        lastTime = SDL_GetTicks();

        if (selectedChair >= 0 && selectedChair < NUM_CHAIRS) {
            if (keyboardState[SDL_SCANCODE_UP]) chairs[selectedChair].z -= 0.05f;
            if (keyboardState[SDL_SCANCODE_DOWN]) chairs[selectedChair].z += 0.05f;
            if (keyboardState[SDL_SCANCODE_LEFT]) chairs[selectedChair].x -= 0.05f;
            if (keyboardState[SDL_SCANCODE_RIGHT]) chairs[selectedChair].x += 0.05f;
        }

        GLfloat lightDiffuse[] = {brightness, brightness, brightness, 1.0f};
        GLfloat lightAmbient[] = {0.3f * brightness, 0.3f * brightness, 0.3f * brightness, 1.0f};
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (!showHelp) {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(70.0, 800.0 / 600.0, 0.1, 100.0);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            camera_apply_transform(&cam);

            drawRoom(floorTexture, wallTexture);

            glBindTexture(GL_TEXTURE_2D, clockTexture);
            glPushMatrix();
            glTranslatef(0.0f, 0.0f, -4.5f);
            glScalef(1.5f, 1.5f, 1.5f);
            drawModel(&clockModel);
            glPopMatrix();

            glBindTexture(GL_TEXTURE_2D, szekTexture);
            for (int i = 0; i < NUM_CHAIRS; ++i) {
                glPushMatrix();
                glTranslatef(chairs[i].x, chairs[i].y, chairs[i].z);
                glRotatef(chairs[i].rotationY, 0, 1, 0);
                glScalef(2.0f, 2.0f, 2.0f);
                drawModel(&szekModel);
                glPopMatrix();
            }

            glBindTexture(GL_TEXTURE_2D, tableTexture);
            glPushMatrix();
            glTranslatef(2.0f, -0.5f, 2.0f);
            glScalef(0.02f, 0.02f, 0.02f);
            drawModel(&tableModel);
            glPopMatrix();
        } else {
            renderHelpOverlay(800, 600);
        }

        SDL_GL_SwapWindow(window);
        SDL_Delay(10);
    }

    freeModel(&clockModel);
    freeModel(&szekModel);
    freeModel(&tableModel);
    glDeleteTextures(1, &clockTexture);
    glDeleteTextures(1, &szekTexture);
    glDeleteTextures(1, &tableTexture);
    glDeleteTextures(1, &floorTexture);
    glDeleteTextures(1, &wallTexture);
    glDeleteTextures(1, &helpTexture);

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
