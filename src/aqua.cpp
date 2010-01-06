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
#include <cstdlib>
#include <GL/glut.h>
#include "model.hpp"

#include "../config.h"

Model fish;
unsigned width = 640, height = 480;
GLfloat angle;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0, -1.0, -6.0);
    glRotatef(angle, 0.0, 1.0, 0.0);

    fish.display();

    glutSwapBuffers();
}

void idle()
{
    angle += 1.0;

    if (angle > 360)
        angle -= 360;

    display();

    usleep(20000);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27:  // The escape key
        case 'Q':
        case 'q':
            exit(0);   // Simply exit
            break;

        case '1':
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;

        case '2':
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;

        case 'l':
            {
                GLboolean lighting;

                glGetBooleanv(GL_LIGHTING, &lighting);

                if (!lighting)
                    glEnable(GL_LIGHTING);
                else
                    glDisable(GL_LIGHTING);

                break;
            }

        case 'g':
            fish.loadObj("goldenfish", 0.012);
            break;

        case 'c':
            fish.loadObj("clownfish", 7.0);
            break;
    }
}

void initGL()
{
    GLfloat light_ambient[] = {0.2, 0.2, 0.2, 1.0};
    GLfloat light_diffuse[] = {0.5, 0.5, 0.5, 1.0};
    GLfloat light_specular[] = {0.9, 0.9, 0.9, 1.0};
    GLfloat light_position[] = {-1.0, 0.0, -6.0, 1.0};

    glClearColor(0.3, 0.3, 0.8, 0.0);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.5);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glDisable(GL_CULL_FACE);
}

void resize(int w, int h)
{
    width = w;
    height = h;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, width/height, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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
    glutKeyboardFunc(keyboard);

    initGL();

    keyboard('c', 0, 0);

    glutMainLoop();

    return EXIT_SUCCESS;
}
