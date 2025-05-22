#include "model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>

Model loadOBJ(const char* filename) {
    Model model = {0};

    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Nem sikerült megnyitni a modellt: %s\n", filename);
        return model;
    }

    float (*temp_vertices)[3] = NULL;
    float (*temp_normals)[3] = NULL;
    float (*temp_texcoords)[2] = NULL;
    int* vertex_indices = NULL;
    int* normal_indices = NULL;
    int* texcoord_indices = NULL;

    int vertex_count = 0, texcoord_count = 0, normal_count = 0, face_count = 0;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "v ", 2) == 0) {
            temp_vertices = realloc(temp_vertices, (vertex_count + 1) * sizeof(*temp_vertices));
            sscanf(line, "v %f %f %f", &temp_vertices[vertex_count][0], &temp_vertices[vertex_count][1], &temp_vertices[vertex_count][2]);
            vertex_count++;
        } else if (strncmp(line, "vt ", 3) == 0) {
            temp_texcoords = realloc(temp_texcoords, (texcoord_count + 1) * sizeof(*temp_texcoords));
            sscanf(line, "vt %f %f", &temp_texcoords[texcoord_count][0], &temp_texcoords[texcoord_count][1]);
            texcoord_count++;
        } else if (strncmp(line, "vn ", 3) == 0) {
            temp_normals = realloc(temp_normals, (normal_count + 1) * sizeof(*temp_normals));
            sscanf(line, "vn %f %f %f", &temp_normals[normal_count][0], &temp_normals[normal_count][1], &temp_normals[normal_count][2]);
            normal_count++;
        } else if (strncmp(line, "f ", 2) == 0) {
            int v[3] = {0}, t[3] = {-1, -1, -1}, n[3] = {-1, -1, -1};
            int matches = sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                                 &v[0], &t[0], &n[0],
                                 &v[1], &t[1], &n[1],
                                 &v[2], &t[2], &n[2]);
            if (matches != 9) {
                matches = sscanf(line, "f %d//%d %d//%d %d//%d",
                                 &v[0], &n[0],
                                 &v[1], &n[1],
                                 &v[2], &n[2]);
                if (matches != 6) {
                    matches = sscanf(line, "f %d/%d %d/%d %d/%d",
                                     &v[0], &t[0],
                                     &v[1], &t[1],
                                     &v[2], &t[2]);
                    if (matches != 6) {
                        matches = sscanf(line, "f %d %d %d",
                                         &v[0], &v[1], &v[2]);
                        if (matches != 3) {
                            fprintf(stderr, "Nem támogatott face formátum: %s", line);
                            continue;
                        }
                    }
                }
            }

            vertex_indices = realloc(vertex_indices, (face_count + 1) * 3 * sizeof(int));
            texcoord_indices = realloc(texcoord_indices, (face_count + 1) * 3 * sizeof(int));
            normal_indices = realloc(normal_indices, (face_count + 1) * 3 * sizeof(int));
            for (int i = 0; i < 3; ++i) {
                vertex_indices[face_count * 3 + i] = v[i] - 1;
                texcoord_indices[face_count * 3 + i] = t[i] - 1;
                normal_indices[face_count * 3 + i] = n[i] - 1;
            }
            face_count++;
        }
    }
    fclose(file);

    model.vertexCount = face_count * 3;
    model.vertices = malloc(model.vertexCount * 3 * sizeof(float));
    model.normals = malloc(model.vertexCount * 3 * sizeof(float));
    model.texcoords = malloc(model.vertexCount * 2 * sizeof(float));

    for (int i = 0; i < model.vertexCount; ++i) {
        int vi = vertex_indices[i];
        int ti = texcoord_indices[i];
        int ni = normal_indices[i];

        memcpy(&model.vertices[i * 3], temp_vertices[vi], 3 * sizeof(float));

        if (ni >= 0 && temp_normals != NULL)
            memcpy(&model.normals[i * 3], temp_normals[ni], 3 * sizeof(float));
        else {
            model.normals[i * 3 + 0] = 0.0f;
            model.normals[i * 3 + 1] = 0.0f;
            model.normals[i * 3 + 2] = 1.0f;
        }

        if (ti >= 0 && temp_texcoords != NULL)
            memcpy(&model.texcoords[i * 2], temp_texcoords[ti], 2 * sizeof(float));
        else {
            model.texcoords[i * 2 + 0] = 0.0f;
            model.texcoords[i * 2 + 1] = 0.0f;
        }
    }

    free(temp_vertices);
    free(temp_normals);
    free(temp_texcoords);
    free(vertex_indices);
    free(normal_indices);
    free(texcoord_indices);

    return model;
}

void drawModel(const Model* model) {
    glBegin(GL_TRIANGLES);
    for (unsigned int i = 0; i < model->vertexCount; ++i) {
        glNormal3fv(&model->normals[i * 3]);
        glTexCoord2fv(&model->texcoords[i * 2]);
        glVertex3fv(&model->vertices[i * 3]);
    }
    glEnd();
}

void freeModel(Model* model) {
    free(model->vertices);
    free(model->normals);
    free(model->texcoords);
}
