//      model.cpp
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


#include "model.hpp"
#include <sstream>
#include <string>
#include <vector>

Model::Model()
{
    clear();
}


Model::~Model()
{
    clear();
}

void Model::clear()
{
    for (std::vector<Buffer>::iterator it = buffer.begin();
        it != buffer.end(); ++it)
    {
        it->clear();
    }

    buffer.clear();

    for (std::map<std::string, Material>::iterator it = material.begin();
        it != material.end(); ++it)
    {
        it->second.clear();
    }

    material.clear();

    min[0] = min[1] = min[2] = 0.0;
    max[0] = max[1] = max[2] = 0.0;

    weight = 1.0;
}

void Model::updateBoundBox(GLfloat v[])
{
    for (int i = 0; i < 3; ++i)
    {
        if (v[i] > max[i])
            max[i] = v[i];

        if (v[i] < min[i])
            min[i] = v[i];
    }

    for (int i = 0; i < 8; ++i)
    {
        if (i == 0 || i == 3 || i == 4 || i == 7)
            boundingBox[3 * i + 0] = max[0];
        else
            boundingBox[3 * i + 0] = min[0];

        if (i <= 3)
            boundingBox[3 * i + 1] = max[1];
        else
            boundingBox[3 * i + 1] = min[1];

        if (i == 0 || i == 1 || i == 4 || i == 5)
            boundingBox[3 * i + 2] = max[2];
        else
            boundingBox[3 * i + 2] = min[2];
    }
}

void Model::addBuffer(const std::vector<GLfloat> &buf,
                        const size_t count, std::string mtl)
{
    if (count > 0)
    {
        Buffer b;
        b.material = mtl;
        b.count = count;
        b.stride = buf.size() / b.count;
        b.vertices = new GLfloat[buf.size()];
        std::copy(buf.begin(), buf.end(), b.vertices);
        buffer.push_back(b);
    }
}

void Model::loadObj(const char *name, GLfloat w)
{
    modelName = name;
    modelPath = std::string(DATADIR "/") + modelName;
    std::string filename = modelPath + std::string("/") + name + std::string(".obj");
    std::ifstream file(filename.c_str());
    std::vector<GLfloat> v_tmp, vt_tmp, buf;
    std::string line, mtl;
    size_t count = 0;

    clear();
    weight = w;

    std::cout << "Loading model from: " << filename << '\n';

    while(getline(file, line))
    {
        if (line.size() > 0 && line[0] != '#')
        {
            std::string command;
            std::istringstream s(line);
            s >> command;

            if (command == "v")
            {
                GLfloat x, y, z;
                s >> x >> y >> z;
                v_tmp.push_back(x);
                v_tmp.push_back(y);
                v_tmp.push_back(z);
            }
            else if (command == "vt")
            {
                GLfloat u, v;
                s >> u >> v;
                vt_tmp.push_back(u);
                vt_tmp.push_back(v);
            }
            else if (command == "f")
            {
                for (unsigned i = 0; i < 3; ++i)
                {
                    char c;
                    unsigned vi, ti;
                    GLfloat v[3], t[2];
                    std::string str;

                    s >> str;
                    std::istringstream(str) >> vi >> c >> ti;

                    --vi;
                    --ti;

                    v[0] = v_tmp[3 * vi];
                    v[1] = v_tmp[3 * vi + 1];
                    v[2] = v_tmp[3 * vi + 2];

                    t[0] = vt_tmp[2 * ti];
                    t[1] = vt_tmp[2 * ti + 1];

                    updateBoundBox(v);

                    buf.push_back(v[0]);
                    buf.push_back(v[1]);
                    buf.push_back(v[2]);

                    buf.push_back(t[0]);
                    buf.push_back(t[1]);

                    ++count;
                }
            }
            else if (command == "mtllib")
            {
                std::string mtllib;
                s >> mtllib;
                loadMaterialLibrary(mtllib.c_str());
            }
            else if (command == "usemtl")
            {
                if (count > 0)
                {
                    addBuffer(buf, count, mtl);
                    buf.clear();
                    count = 0;
                }

                s >> mtl;
            }
            else
            {
                std::cout << '\'' << command << "' not implemented\n";
            }
        }
    }

    addBuffer(buf, count, mtl);
}

void Model::loadMaterialLibrary(const char *mtlfile)
{
    std::string filename = modelPath + std::string("/") + mtlfile;
    std::ifstream file(filename.c_str());

    std::cout << "Loading material library: " << filename << '\n';

    std::string line;
    std::string mtlname;

    while(getline(file, line))
    {
        if (line.size() > 0 && line[0] != '#')
        {
            std::string command;
            std::istringstream s(line);
            s >> command;

            if (command == "newmtl")
            {
                s >> mtlname;
                material[mtlname] = Material();
            }
            else if (command == "Ka")
            {
                Material &mtl = material[mtlname];
                s >> mtl.color_ambient[0] >> mtl.color_ambient[1]
                    >> mtl.color_ambient[2];
            }
            else if (command == "Kd")
            {
                Material &mtl = material[mtlname];
                s >> mtl.color_diffuse[0] >> mtl.color_diffuse[1]
                    >> mtl.color_diffuse[2];
            }
            else if (command == "Ks")
            {
                Material &mtl = material[mtlname];
                s >> mtl.color_specular[0] >> mtl.color_specular[1]
                    >> mtl.color_specular[2];
            }
            else if (command == "map_Kd")
            {
                std::string texname;
                s >> texname;
                Material &mtl = material[mtlname];
                mtl.loadTexture(modelName + "/" + texname);
            }
            else
            {
                std::cout << "Material command " << command
                    << " not implemented\n";
            }
        }
    }
}

void Model::display()
{
    glPushMatrix();

    glScalef(weight, weight, weight);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    for (std::vector<Buffer>::iterator it = buffer.begin();
        it != buffer.end(); ++it)
    {
        glVertexPointer(3, GL_FLOAT, sizeof(GLfloat) * it->stride, it->vertices);
        glTexCoordPointer(2, GL_FLOAT, sizeof(GLfloat) * it->stride, it->vertices + 3);
        useMaterial(it->material);
        glDrawArrays(it->mode, 0, it->count);
    }
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    GLint mode[2];
    glGetIntegerv(GL_POLYGON_MODE, mode);
    glBindTexture(GL_TEXTURE_2D, 0);
    if (mode[0] == GL_LINE)
    {
        static GLubyte indices[] = {0, 1, 2, 3,
                                      4, 5, 1, 0,
                                      5, 6, 2, 1,
                                      6, 7, 3, 2,
                                      7, 4, 0, 3,
                                      5, 4, 7, 6};

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0,  boundingBox);
        glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, indices);
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    glPopMatrix();
}

void Model::useMaterial(const std::string &mtl)
{
    try
    {
        Material &m = material[mtl];
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m.color_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m.color_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m.color_specular);
        glBindTexture(GL_TEXTURE_2D, m.texture);
    }
    catch(...)
    {
        std::cout << "Cannot set material " << mtl << '\n';
    }
}
