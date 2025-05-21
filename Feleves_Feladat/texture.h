#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/gl.h>

GLuint loadTexture(const char* filename);
GLuint loadBMPTexture(const char* filepath);
void drawWall(float x, float y, float z, float width, float height, GLuint texture);
void drawFloor(float x, float y, float z, float width, float depth, GLuint texture);

#endif
