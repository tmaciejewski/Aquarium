//      model.hpp
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


#ifndef MODEL_H
#define MODEL_H

#include <fstream>
#include <GL/gl.h>
#include <GL/glext.h>

class Model
{
    public:
        Model();
        virtual ~Model();

        void loadObj(const char *name, GLfloat w = 1.0);

        const GLfloat *getBuffer() { return buffer; }
        unsigned getCount() { return count; }
        unsigned getStride() { return stride; }

    private:
        GLfloat *buffer;
        GLfloat min[3], max[3], weight;
        size_t count;
        unsigned stride;

        void clear();

};

#endif /* MODEL_H */
