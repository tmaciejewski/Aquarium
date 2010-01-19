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

void Fish::display()
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
    if (state == S_MOVING)
    {
        //static GLfloat t = 0;
        //hAngle += 0.05;

        if (hAngle > 2 * M_PI)
        {
            hAngle -= 2 * M_PI;
        }

        //vAngle = M_PI / 4.0 * sin(t);

        //t += 0.02;

        //if (t > 2 * M_PI)
            //t -= 2 * M_PI;

        swim(0.1);

        if (x > 10.0 || x < -10.0)
        {
            x = (x > 0 ? 10.0 : -10.0);
            hAngle += M_PI;
        }
    }
}
