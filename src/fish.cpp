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

Fish::Fish(const Model *m, GLfloat s) : model(m), scale(s), state(S_MOVING)
{
    pos[0] = pos[1] = pos[2] = 0.0;
    oldPos[0] = oldPos[1] = oldPos[2] = 0.0;
    hAngle = 2 * M_PI * (rand() / (float) RAND_MAX - 0.5);
    vAngle = M_PI_2 * (rand() / (float) RAND_MAX - 0.5);
}

Fish::~Fish()
{

}

void Fish::display() const
{
    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2]);
    glRotatef(hAngle * (180.0 / M_PI), 0.0, 1.0, 0.0);
    glRotatef(-vAngle * (180.0 / M_PI), 0.0, 0.0, 1.0);
    if (model)
        model->display(scale);
    glPopMatrix();
}

void Fish::update()
{
    static GLfloat speed = 0.5, turnSpeed = speed / 5.0;
    static GLfloat len, turn[2];
    static short direction[2];
    static State lastState = state;

    std::copy(pos, pos + 3, oldPos);

    if (len <= 0)
    {
        state = S_MOVING;
        len = 1.0 + 10.0 * (rand() / (float) RAND_MAX);
        turn[0] = M_PI * (rand() / (float) RAND_MAX);
        turn[1] = M_PI_4 * (rand() / (float) RAND_MAX);
        direction[0] = rand() % 3 - 1;
        direction[1] = rand() % 3 - 1;
    }

    if (state == S_MOVING)
    {
        len -= speed;

        turn[0] -= turnSpeed;
        turn[1] -= turnSpeed;

        if (turn[0] > 0)
            hAngle += direction[0] * turnSpeed;

        if (turn[1] > 0)
            vAngle += direction[1] * turnSpeed;

        swim(speed);
    }
    else if (state == S_COLLISION)
    {
        if (lastState != state)
        {
            hAngle = hAngle + M_PI;
            vAngle = -vAngle;
        }

        swim(speed);
    }

    lastState = state;
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
