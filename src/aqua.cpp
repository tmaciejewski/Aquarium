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
#include <iostream>
#include <GL/glu.h>
#include <GL/gl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "modellib.hpp"

#include "../config.h"

ModelLib modelLib;
unsigned width = 640, height = 480;
GLfloat angle, scale = 7.0;
GLuint waterTex;
std::vector<bool> keyPressed(SDLK_LAST);
SDL_Surface *surface;
bool lighting;
std::string model = "clownfish";

SDL_Surface * setVideoMode()
{
    SDL_Surface *surface;
    int flags = SDL_OPENGL | SDL_RESIZABLE;
    if ((surface = SDL_SetVideoMode(width, height, 0, flags)) == NULL)
    {
        std::cerr << "Unable to create OpenGL screen: " << SDL_GetError() << '\n';
        SDL_Quit();
        exit(-1);
    }

    return surface;
}

void setWaterTexture()
{
    SDL_Surface *water = IMG_Load(DATADIR "/water.jpg");
    if (water)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, water->w,
                    water->h, 0, GL_RGB, GL_UNSIGNED_BYTE,
                    water->pixels);
        SDL_FreeSurface(water);
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glTranslatef(0.0, 0.0, -1.1);
    glBindTexture(GL_TEXTURE_2D, waterTex);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0, 1.0); glVertex3f(-1.0, -1.0, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(-1.0, 1.0, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(1.0, 1.0, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(1.0, -1.0, 0.0);
    glEnd();

    glClear(GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(0.0, -1.0, -5.0);
    glRotatef(angle, 0.0, 1.0, 0.0);

    if (lighting)
        glEnable(GL_LIGHTING);

    modelLib.display(model, scale);

    SDL_GL_SwapBuffers();
}

void initGL()
{
    GLfloat light_ambient[] = {0.3, 0.3, 0.3, 1.0};
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

    glEnable(GL_LIGHT0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glDisable(GL_CULL_FACE);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glEnable(GL_TEXTURE_2D);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &waterTex);
    glBindTexture(GL_TEXTURE_2D, waterTex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    setWaterTexture();
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

void update()
{
    if(keyPressed['1'])
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if(keyPressed['2'])
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    if(keyPressed['l'])
    {
        lighting = !lighting;
        keyPressed['l'] = false;
    }

    if(keyPressed['g'])
    {
        model = "goldenfish";
        scale = 0.01;
        keyPressed['g'] = false;
    }

    if (keyPressed['c'])
    {
        model = "clownfish";
        scale = 7.0;
        keyPressed['c'] = false;
    }

    if (keyPressed['b'])
    {
        model = "ladybug";
        scale = 0.005;
        keyPressed['b'] = false;
    }

    angle += 1.0;

    if (angle > 360)
        angle -= 360;
}

void run()
{
    SDL_Event event = {0};

    while (!keyPressed[SDLK_ESCAPE] && event.type != SDL_QUIT)
    {
        update();
        display();

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
            {
                keyPressed[event.key.keysym.sym] = static_cast<bool>(event.key.state);
            }

            if (event.type == SDL_VIDEORESIZE)
            {
                width = event.resize.w;
                height = event.resize.h;

                if (surface)
                    SDL_FreeSurface(surface);

                surface = setVideoMode();
                resize(width, height);
            }
        }

        usleep(20000);
    }
}

int main(int argc, char **argv)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Unable to initialize SDL: " << SDL_GetError() << '\n';
        return -1;
    }

    IMG_Init(IMG_INIT_JPG);

    surface = setVideoMode();

    SDL_WM_SetCaption(PACKAGE_STRING, NULL);

    initGL();
    resize(640, 480);

    modelLib.loadLib(DATADIR);
    modelLib.loadLib(".");

    run();

    IMG_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}
