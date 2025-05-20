#include "texture.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <GL/gl.h>

GLuint loadTexture(const char* filename) {
    SDL_Surface* surface = IMG_Load(filename);
    if (!surface) {
        fprintf(stderr, "IMG_Load failed: %s\n", IMG_GetError());
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    GLint mode = GL_RGB;
    if (surface->format->BytesPerPixel == 4) mode = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, mode,
                 surface->w, surface->h,
                 0, mode, GL_UNSIGNED_BYTE, surface->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_FreeSurface(surface);

    return textureID;
}
GLuint loadBMPTexture(const char* filepath) {
    SDL_Surface* surface = SDL_LoadBMP(filepath);
    if (!surface) {
        fprintf(stderr, "SDL_LoadBMP error: %s\n", SDL_GetError());
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);


    GLint format = (surface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, surface->w, surface->h, 0,
                 format, GL_UNSIGNED_BYTE, surface->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_FreeSurface(surface);
    return textureID;
}

void drawWall(float x, float y, float z, float width, float height, GLuint texture) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(x, y, z);
    glTexCoord2f(1, 0); glVertex3f(x + width, y, z);
    glTexCoord2f(1, 1); glVertex3f(x + width, y + height, z);
    glTexCoord2f(0, 1); glVertex3f(x, y + height, z);
    glEnd();
}

void drawFloor(float x, float y, float z, float width, float depth, GLuint texture) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(x, y, z);
    glTexCoord2f(1, 0); glVertex3f(x + width, y, z);
    glTexCoord2f(1, 1); glVertex3f(x + width, y, z + depth);
    glTexCoord2f(0, 1); glVertex3f(x, y, z + depth);
    glEnd();
}


