#ifndef MODEL_H
#define MODEL_H

#include <GL/gl.h>

typedef struct {
    float* vertices;
    float* normals;
    float* texcoords;
    unsigned int vertexCount;
} Model;

Model loadOBJ(const char* filename);
void drawModel(const Model* model);
void freeModel(Model* model);

#endif
