//      fish.hpp
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


#ifndef FISH_HPP
#define FISH_HPP

#include "modellib.hpp"
#include <cmath>

class Fish
{
    public:

        enum State { S_STOP, S_MOVING, S_COLLISION };

        Fish(const Model *m = NULL, GLfloat s = 1.0);
        virtual ~Fish();

        void display();
        void update();

        void setModel(const Model *m)
        {
            model = m;
        }

        void setXYZ(GLfloat _x, GLfloat _y, GLfloat _z)
        {
            x = _x; y = _y; z = _z;
        }

        void turn(GLfloat h, GLfloat v)
        {
            hAngle += h;
            vAngle += v;
        }

        void swim(GLfloat len = 1.0)
        {
            y += len * sin(vAngle);
            x -= len * cos(vAngle)*cos(hAngle);
            z += len * cos(vAngle)*sin(hAngle);
        }

    private:
        const Model *model;
        GLfloat x, y, z, hAngle, vAngle, scale;
        State state;
};

#endif /* FISH_HPP */