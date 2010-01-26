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

    for (std::map<std::string, GLuint>::iterator it = texture.begin();
        it != texture.end(); ++it)
    {
        glDeleteTextures(1, &it->second);
    }

    material.clear();
    texture.clear();

    min[0] = min[1] = min[2] = 0.0;
    max[0] = max[1] = max[2] = 0.0;
}

void Model::addBuffer(const std::vector<GLfloat> &buf,
                        const size_t count, std::string mtl)
{
    if (count > 0)
    {
        Buffer b;
        b.material = mtl;
        b.count = count;
        b.vertices = new GLfloat[buf.size()];
        std::copy(buf.begin(), buf.end(), b.vertices);
        buffer.push_back(b);
    }
}

void Model::loadObj(const char *path, const char *name)
{
    modelPath = path;
    std::string filename = modelPath + "/" + std::string(name) + ".obj";
    std::ifstream file(filename.c_str());
    std::vector<GLfloat> tmp_buf[3], buf;
    std::string line, mtl;
    size_t count = 0;

    clear();

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
                tmp_buf[0].push_back(x);
                tmp_buf[0].push_back(y);
                tmp_buf[0].push_back(z);
            }
            else if (command == "vn")
            {
                GLfloat x, y, z;
                s >> x >> y >> z;
                tmp_buf[2].push_back(x);
                tmp_buf[2].push_back(y);
                tmp_buf[2].push_back(z);
            }
            else if (command == "vt")
            {
                GLfloat u, v;
                s >> u >> v;
                tmp_buf[1].push_back(u);
                tmp_buf[1].push_back(v);
            }
            else if (command == "f")
            {
                count += addFace(buf, tmp_buf, s);
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
            else if (command == "" || command == "g" || command == "s"
                       || command == "o")
            {

            }
            else
            {
                std::cout << '\'' << command << "' not implemented\n";
            }
        }
    }

    addBuffer(buf, count, mtl);
}

void Model::updateBoundBox(const GLfloat v[])
{
    for (int i = 0; i < 3; ++i)
    {
        if (v[i] > max[i])
            max[i] = v[i];

        if (v[i] < min[i])
            min[i] = v[i];

        size[i] = max[i] - min[i];
        if (size[i] < 0) size[i] = -size[i];
        center[i] = min[i] + size[i] / 2.0;
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

void Model::addVertex(std::vector<GLfloat> &buf,
            const std::vector<GLfloat> *tmp_buf, unsigned *index)
{
    GLfloat v[3], t[2] = {0}, n[3] = {0.0, 0.0, 1.0};
    unsigned vi = index[0] - 1, ti, ni;

    v[0] = tmp_buf[0][3 * vi];
    v[1] = tmp_buf[0][3 * vi + 1];
    v[2] = tmp_buf[0][3 * vi + 2];

    updateBoundBox(v);

    if (index[1] > 0)
    {
        ti = index[1] - 1;
        t[0] = tmp_buf[1][2 * ti];
        t[1] = tmp_buf[1][2 * ti + 1];
    }

    if (index[2] > 0)
    {
        ni = index[2] - 1;
        n[0] = tmp_buf[2][3 * ni];
        n[1] = tmp_buf[2][3 * ni + 1];
        n[2] = tmp_buf[2][3 * ni + 2];
    }
    else
    {
        n[0] = v[0];
        n[1] = v[1];
        n[2] = v[2];
    }

    buf.push_back(t[0]);
    buf.push_back(t[1]);

    buf.push_back(n[0]);
    buf.push_back(n[1]);
    buf.push_back(n[2]);

    buf.push_back(v[0]);
    buf.push_back(v[1]);
    buf.push_back(v[2]);
}

unsigned Model::parseIndices(std::istream &s, unsigned *index, size_t n)
{
    unsigned i = 0;
    std::string token;
    s >> token;
    std::istringstream tokenStream(token);
    do {
        tokenStream >> index[i];
        ++i;
    } while (tokenStream.get() == '/' && i < n);

    return i;
}

unsigned Model::addFace(std::vector<GLfloat> &buf,
            const std::vector<GLfloat> *tmp_buf, std::istream &s)
{
    unsigned count = 0, firstVertex[3] = {0}, lastVertex[3] = {0};

    parseIndices(s, firstVertex);
    parseIndices(s, lastVertex);

    while(s)
    {
        unsigned vertex[3] = {0};

        parseIndices(s, vertex);

        if (s)
        {
            addVertex(buf, tmp_buf, firstVertex);
            addVertex(buf, tmp_buf, lastVertex);
            addVertex(buf, tmp_buf, vertex);

            std::copy(vertex, vertex + 3, lastVertex);

            count += 3;
        }
    }

    return count;
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
            }
            else if (mtlname != "")
            {
                Material &mtl = material[mtlname];

                if (command == "Ka")
                {
                    s >> mtl.color_ambient[0] >> mtl.color_ambient[1]
                        >> mtl.color_ambient[2];
                }
                else if (command == "Kd")
                {
                    s >> mtl.color_diffuse[0] >> mtl.color_diffuse[1]
                        >> mtl.color_diffuse[2];
                }
                else if (command == "Ks")
                {
                    s >> mtl.color_specular[0] >> mtl.color_specular[1]
                        >> mtl.color_specular[2];
                }
                else if (command == "Ns")
                {
                    s >> mtl.ns;
                }
                else if (command == "d")
                {
                    s >> mtl.d;
                }
                else if (command == "illum")
                {
                    s >> mtl.illum;
                }
                else if (command == "map_Kd")
                {
                    s >> mtl.texture;
                    loadTexture(mtl.texture);
                }
                else if (command != "")
                {
                    std::cout << "Material command " << command
                        << " not implemented\n";
                }
            }
        }
    }
}

void Model::display(const GLfloat scale) const
{
    glPushMatrix();

    glTranslatef(-center[0] * scale, -center[1] * scale, -center[2] * scale);
    glScalef(scale, scale, scale);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    for (std::vector<Buffer>::const_iterator it = buffer.begin();
        it != buffer.end(); ++it)
    {
        useMaterial(it->material);
        glInterleavedArrays(GL_T2F_N3F_V3F, 0, it->vertices);
        glDrawArrays(it->mode, 0, it->count);
    }
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

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

void Model::useMaterial(const std::string &mtl) const
{
    std::map<std::string, Material>::const_iterator matIt = material.find(mtl);
    if (matIt != material.end())
    {
        const Material &m = matIt->second;
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m.color_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m.color_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m.color_specular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m.ns);

        std::map<std::string, GLuint>::const_iterator texIt = texture.find(m.texture);
        if (texIt != texture.end())
            glBindTexture(GL_TEXTURE_2D, texIt->second);
        else
            glBindTexture(GL_TEXTURE_2D, 0);
    }
    else
    {
        // default material
        GLfloat mat_ambient[] = {0.2, 0.2, 0.2, 1.0};
        GLfloat mat_diffuse[] = {0.6, 0.6, 0.6, 1.0};
        GLfloat mat_specular[] = {0.5, 0.5, 0.5, 1.0};
        GLfloat mat_shininess[] = {1.0};

        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Model::loadTexture(const std::string &texname)
{
    if (texture.find(texname) == texture.end())
    {
        std::string filename = modelPath + "/" + texname;
        std::cout << "Loading texture " << texname << " from " << filename << '\n';
        SDL_Surface *img = IMG_Load(filename.c_str());

        if (img)
        {
            GLuint t;
            glGenTextures(1, &t);
            texture[texname] = t;
            glBindTexture(GL_TEXTURE_2D, texture[texname]);
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w,
                        img->h, 0, GL_RGB, GL_UNSIGNED_BYTE,
                        img->pixels);
            SDL_FreeSurface(img);
        }
    }
}

void Model::normalize()
{
    for (std::vector<Buffer>::const_iterator it = buffer.begin();
        it != buffer.end(); ++it)
    {
        for (int i = 0; i < it->count; i += 8)
        {

        }
    }
}
