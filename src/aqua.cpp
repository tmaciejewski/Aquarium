//      aqua.cpp
//
//      Copyright 2010 Tomasz Maciejewski <ponton@jabster.pl>
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.

#include <unistd.h>
#include <GL/glut.h>
#include "model.hpp"

#include "../config.h"

Model fish;
unsigned width = 800, height = 600;
GLfloat angle;

void resize(int w, int h)
{
    width = w;
    height = h;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, width/height, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0, 0.0, -5.0);
    glRotatef(angle, 0.0, 1.0, 0.0);


    glColor3f(0.2, 0.4, 0.6);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(4, GL_FLOAT, sizeof(GLfloat)*fish.getStride(), fish.getBuffer());
    glDrawArrays(GL_TRIANGLES, 0, fish.getCount());
    glDisableClientState(GL_VERTEX_ARRAY);

    glutSwapBuffers();
}

void idle()
{
    angle += 10.0;

    if (angle > 360)
        angle -= 360;

    display();

    usleep(20000);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow(PACKAGE_STRING);

    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutIdleFunc(idle);

    glEnable(GL_DEPTH);

    fish.loadObj(DATADIR "/clownfish/clownfish.obj", 0.5);

    glutMainLoop();

    return 0;
}
