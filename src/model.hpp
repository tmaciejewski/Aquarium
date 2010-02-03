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
#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <GL/gl.h>
#include <GL/glext.h>
#include <SDL/SDL_image.h>

class Model
{
    public:
        Model();
        virtual ~Model();

        void loadObj(const char *path, const char *name);
        void display(const GLfloat scale = 1.0) const;

        GLfloat getCenter(unsigned i) const { return center[i]; }
        GLfloat getSize(unsigned i) const { return size[i]; }
        const GLfloat *getBBox() const { return boundingBox; }

    private:

        struct Material
        {
            GLfloat color_ambient[3], color_diffuse[3], color_specular[3];
            GLfloat ns, d;
            unsigned illum;
            std::string texture;
        };

        struct Buffer
        {
            GLenum mode;
            GLfloat *vertices;
            size_t count;
            std::string material;

            Buffer() : mode(GL_TRIANGLES), vertices(NULL) {}
            void clear() { if (vertices) delete[] vertices; }
        };

        std::vector<Buffer> buffer;
        GLfloat boundingBox[24];
        GLfloat min[3], max[3], center[3], size[3];

        std::string modelPath;
        std::map<std::string, Material> material;
        std::map<std::string, GLuint> texture;

        void clear();
        void updateBoundBox(const GLfloat v[]);
        void loadMaterialLibrary(const char *mtlfile);
        void loadTexture(const std::string &texname);
        void addVertex(std::vector<GLfloat> &buf,
            const std::vector<GLfloat> *tmp_buf, unsigned *index);
        unsigned addFace(std::vector<GLfloat> &buf,
            const std::vector<GLfloat> *tmp_buf, std::istream &s);
        unsigned parseIndices(std::istream &s, unsigned *index, size_t n = 3);
        void verticesFromIndex(unsigned vi, unsigned ti, GLfloat *v, GLfloat *t);
        void addBuffer(const std::vector<GLfloat> &buf,
            const size_t count, std::string mtl);
        void useMaterial(const std::string &mtl) const;
};

#endif /* MODEL_H */
