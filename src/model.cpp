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
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

Model::Model() : buffer(NULL)
{
    clear();
}


Model::~Model()
{
    clear();
}

void Model::clear()
{
    if (buffer)
        delete[] buffer;

    min[0] = min[1] = min[2] = 0.0;
    max[0] = max[1] = max[2] = 0.0;

    weight = 1.0;

    count = 0;
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

void Model::loadObj(const char *filename, GLfloat w)
{
    std::ifstream file(filename);
    std::vector<GLfloat> v_tmp, vt_tmp, buf;
    std::string line;

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
                    buf.push_back(weight);

                    buf.push_back(t[0]);
                    buf.push_back(t[1]);

                    ++count;
                }
            }
            else
            {
                std::cout << '\'' << command << "' not implemented\n";
            }
        }
    }

    if (count > 0)
    {
        stride = buf.size() / count;

        buffer = new GLfloat[buf.size()];
        std::copy(buf.begin(), buf.end(), buffer);


    }
}
