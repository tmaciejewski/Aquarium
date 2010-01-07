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

        void loadObj(const char *name, GLfloat w = 1.0);
        void display();

    private:

        struct Material
        {
            GLfloat color_ambient[3], color_diffuse[3], color_specular[3];
            GLuint texture;

            Material()
            {
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                               GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                               GL_NEAREST);
            }

            void clear() { glDeleteTextures(1, &texture); }

            void loadTexture(const std::string &texname)
            {
                std::string filename = std::string(DATADIR"/") + texname;
                std::cout << "Loading texture: " << filename << '\n';
                SDL_Surface *img = IMG_Load(filename.c_str());
                glBindTexture(GL_TEXTURE_2D, texture);
                if (img)
                {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w,
                                img->h, 0, GL_RGB, GL_UNSIGNED_BYTE,
                                img->pixels);
                    SDL_FreeSurface(img);
                }
            }
        };

        struct Buffer
        {
            GLenum mode;
            GLfloat *vertices;
            size_t count, stride;
            std::string material;

            Buffer() : vertices(NULL), mode(GL_TRIANGLES) {}
            void clear() { if (vertices) delete[] vertices; }
        };

        std::vector<Buffer> buffer;
        GLfloat boundingBox[24];
        GLfloat min[3], max[3], weight;

        std::string modelName, modelPath;
        std::map<std::string, Material> material;

        void clear();
        void updateBoundBox(GLfloat v[]);
        void loadMaterialLibrary(const char *mtlfile);
        void addBuffer(const std::vector<GLfloat> &buf,
            const size_t count, std::string mtl);
        void useMaterial(const std::string &mtl);

};

#endif /* MODEL_H */
