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

        void display() const;
        void update();

        bool collides(const Fish *f) const;

        GLfloat getScale() const
        {
            return scale;
        }

        const Model *getModel() const
        {
            return model;
        }

        void setModel(const Model *newModel)
        {
            model = newModel;
        }

        State getState() const
        {
            return state;
        }

        void setState(State newState)
        {
            state = newState;
        }

        void setXYZ(GLfloat x, GLfloat y, GLfloat z)
        {
            pos[0] = x; pos[1] = y; pos[2] = z;
        }

        const GLfloat *getXYZ() const
        {
            return pos;
        }

        const GLfloat *getAngle() const
        {
            return angle;
        }

        void swim(GLfloat len = 1.0)
        {
            pos[1] += len * sin(angle[1]);
            pos[0] -= len * cos(angle[1])*cos(angle[0]);
            pos[2] += len * cos(angle[1])*sin(angle[0]);
        }

        void undoSwim()
        {
            std::copy(oldPos, oldPos + 3, pos);
        }

    private:
        const Model *model;
        GLfloat pos[3], oldPos[3], angle[2], swimLen, nextAngle[2], scale;
        State state, lastState;
};

#endif /* FISH_HPP */
