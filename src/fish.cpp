//      fish.cpp
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


#include "fish.hpp"

Fish::Fish(const Model *m, GLfloat s)
    : model(m), swimLen(0.0), scale(s), state(S_MOVING), lastState(state)
{
    pos[0] = pos[1] = pos[2] = 0.0;
    oldPos[0] = oldPos[1] = oldPos[2] = 0.0;
    angle[0] = 2 * M_PI * (rand() / (float) RAND_MAX - 0.5);
    angle[1] = M_PI_2 * (rand() / (float) RAND_MAX - 0.5);
}

Fish::~Fish()
{

}

void Fish::display() const
{
    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2]);
    glRotatef(angle[0] * (180.0 / M_PI), 0.0, 1.0, 0.0);
    glRotatef(-angle[1] * (180.0 / M_PI), 0.0, 0.0, 1.0);
    if (model)
        model->display(scale);
    glPopMatrix();
}

void Fish::update()
{
    static GLfloat speed = 0.15, turnSpeed = speed / 5.0;

    if (swimLen <= 0)
    {
        swimLen = 1.0 + 10.0 * (rand() / (float) RAND_MAX);
        nextAngle[0] = 2 * M_PI * (rand() / (float) RAND_MAX - 0.5);
        nextAngle[1] = M_PI_2 * (rand() / (float) RAND_MAX - 0.5);
    }

    if (state == S_MOVING)
    {
        GLfloat delta[] = {angle[0] - nextAngle[0], angle[1] - nextAngle[1]};
        swimLen -= speed;

        for (int i = 0; i < 2; ++i)
        {
            GLfloat delta = angle[i] - nextAngle[i];

            if (delta > turnSpeed)
                angle[i] -= turnSpeed;
            else if (delta < -turnSpeed)
                angle[i] += turnSpeed;
            else
                angle[i] = nextAngle[i];
        }

        swim(speed);
    }
    else if (state == S_COLLISION)
    {
        if (lastState != state)
        {
            nextAngle[0] = angle[0] = angle[0] + M_PI;
            nextAngle[1] = angle[1] = -angle[1];
        }

        swim(speed);
    }

    lastState = state;
    std::copy(pos, pos + 3, oldPos);
}

bool Fish::collides(const Fish *f) const
{
    if (model && f->model)
    {
        for (int i = 0; i < 3; ++i)
        {
            GLfloat dist = pos[i] + model->getCenter(i)
                            - f->model->getCenter(i) - f->pos[i];
            if (dist < 0)
                dist = -dist;

            if (dist >= (model->getSize(i) * scale + f->scale * f->model->getSize(i)) / 2.0)
            {
                return false;
            }
        }

        return true;
    }
    else
        return false;
}
