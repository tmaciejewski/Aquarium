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
    : model(m), scale(s), vAngle(0.0),
      hAngle(0.0), state(S_MOVING)
{
    pos[0] = pos[1] = pos[2] = 0.0;
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
    static GLfloat speed = 0.05;
    static State lastState = state;
    if (state == S_MOVING)
    {
        static GLfloat len = 1.0;

        if (len <= 0)
        {
            len = 1.0 + 5.0 * (rand() / (float) RAND_MAX);
            hAngle += M_PI * (2.0 * (rand() / (float) RAND_MAX) - 1.0);
        }

        swim(speed);
        len -= speed;

        if (pos[0] > 100.0 || pos[0] < -100.0)
        {
            state = S_COLLISION;
        }
    }
    else if (state == S_COLLISION)
    {
        static GLfloat turn = 0.05;
        if (lastState == S_MOVING)
            turn = turn * (rand() % 2 == 1 ? 1 : -1);

        //hAngle += turn;
        hAngle = -hAngle;
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
