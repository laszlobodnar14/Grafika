#include "room.h"

void drawRoom(GLuint floorTex, GLuint wallTex) {
    glColor3f(1.0f, 1.0f, 1.0f);


    glBindTexture(GL_TEXTURE_2D, floorTex);
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0, 0); glVertex3f(-5.0f, 0.0f, -5.0f);
        glTexCoord2f(5, 0); glVertex3f(5.0f, 0.0f, -5.0f);
        glTexCoord2f(5, 5); glVertex3f(5.0f, 0.0f, 5.0f);
        glTexCoord2f(0, 5); glVertex3f(-5.0f, 0.0f, 5.0f);
    glEnd();


    glBindTexture(GL_TEXTURE_2D, wallTex);


    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0, 0); glVertex3f(-5.0f, 0.0f, -5.0f);
        glTexCoord2f(5, 0); glVertex3f(5.0f, 0.0f, -5.0f);
        glTexCoord2f(5, 3); glVertex3f(5.0f, 3.0f, -5.0f);
        glTexCoord2f(0, 3); glVertex3f(-5.0f, 3.0f, -5.0f);
    glEnd();


    glBegin(GL_QUADS);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0, 0); glVertex3f(5.0f, 0.0f, -5.0f);
        glTexCoord2f(5, 0); glVertex3f(5.0f, 0.0f, 5.0f);
        glTexCoord2f(5, 3); glVertex3f(5.0f, 3.0f, 5.0f);
        glTexCoord2f(0, 3); glVertex3f(5.0f, 3.0f, -5.0f);
    glEnd();


    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(0, 0); glVertex3f(5.0f, 0.0f, 5.0f);
        glTexCoord2f(5, 0); glVertex3f(-5.0f, 0.0f, 5.0f);
        glTexCoord2f(5, 3); glVertex3f(-5.0f, 3.0f, 5.0f);
        glTexCoord2f(0, 3); glVertex3f(5.0f, 3.0f, 5.0f);
    glEnd();


    glBegin(GL_QUADS);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(0, 0); glVertex3f(-5.0f, 0.0f, 5.0f);
        glTexCoord2f(5, 0); glVertex3f(-5.0f, 0.0f, -5.0f);
        glTexCoord2f(5, 3); glVertex3f(-5.0f, 3.0f, -5.0f);
        glTexCoord2f(0, 3); glVertex3f(-5.0f, 3.0f, 5.0f);
    glEnd();


    glBegin(GL_QUADS);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glTexCoord2f(0, 0); glVertex3f(-5.0f, 3.0f, -5.0f);
        glTexCoord2f(5, 0); glVertex3f(5.0f, 3.0f, -5.0f);
        glTexCoord2f(5, 5); glVertex3f(5.0f, 3.0f, 5.0f);
        glTexCoord2f(0, 5); glVertex3f(-5.0f, 3.0f, 5.0f);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
}
