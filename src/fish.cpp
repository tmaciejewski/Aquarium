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
    : model(m), x(0.0), y(0.0), z(-5.0), scale(s), vAngle(0.0),
      hAngle(0.0), state(S_MOVING)
{
}

Fish::~Fish()
{

}

void Fish::display() const
{
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(hAngle * (180.0 / M_PI), 0.0, 1.0, 0.0);
    glRotatef(-vAngle * (180.0 / M_PI), 0.0, 0.0, 1.0);
    if (model)
        model->display(scale);
    glPopMatrix();
}

void Fish::update()
{
    static GLfloat speed = 0.1;
    static GLfloat len = 1.0;
    if (state == S_MOVING)
    {
        if (len <= 0)
        {
            len = 1.0 + 5.0 * (rand() / (float) RAND_MAX);
            hAngle += M_PI * (2.0 * (rand() / (float) RAND_MAX) - 1.0);
        }

        swim(speed);
        len -= speed;

        if (x > 100.0 || x < -100.0)
        {
            state = S_COLLISION;
        }
    }
    else if (state == S_COLLISION)
    {
        //hAngle += M_PI;
        swim(-speed);
        //state = S_MOVING;
    }
}

bool Fish::collides(const Fish *f) const
{
    if (model && f->model)
    {
        const GLfloat *bbox;
        bbox = f->model->getBBox();
        for (int i = 0; i < 8; ++i)
        {
            GLfloat dx = (f->x - x) + f->scale * bbox[3 * i];
            GLfloat dy = (f->y - y) + f->scale * bbox[3 * i + 1];
            GLfloat dz = (f->z - z) + f->scale * bbox[3 * i + 2];

            if (model->collides(dx / scale, dy / scale, dz / scale))
                return true;
        }
    }
    else
        return false;
}
