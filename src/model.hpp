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
#include <map>
#include <memory>
#include <vector>
#include <GL/gl.h>
#include <GL/glext.h>

class Model
{
    public:
        Model();
        virtual ~Model();

        void loadObj(const char *name, GLfloat w = 1.0);
        void display();

    private:

        struct Material
        {
            GLfloat color_ambient[3], color_diffuse[3], color_specular[3];
        };

        struct Buffer
        {
            GLenum mode;
            GLfloat *vertices;
            size_t count, stride;

            Buffer() : vertices(NULL), mode(GL_TRIANGLES) {}
            ~Buffer() { if (vertices) delete[] vertices; }
        };

        std::vector<Buffer> buffer;
        GLfloat boundingBox[24];
        GLfloat min[3], max[3], weight;

        std::string modelName, modelPath;
        std::map<std::string, Material> material;

        void clear();
        void updateBoundBox(GLfloat v[]);
        void loadMaterialLibrary(const char *mtlfile);

};

#endif /* MODEL_H */
