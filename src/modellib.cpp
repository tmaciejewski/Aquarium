//      modellib.cpp
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


#include "modellib.hpp"
#include <boost/filesystem.hpp>

ModelLib::ModelLib()
{

}

ModelLib::~ModelLib()
{
    for (std::map<std::string, Model *>::iterator it = model.begin();
        it != model.end() ; ++it)
    {
        delete it->second;
    }
}

void ModelLib::display(const std::string &modelName, GLfloat scale)
{
    model[modelName]->display(scale);
}

void ModelLib::loadLib(const char *p)
{
    namespace fs = boost::filesystem;
    fs::path path(p);
    fs::recursive_directory_iterator it(path), end;
    while (it != end)
    {
        if (fs::is_regular(it->status()) && it->path().extension() == ".obj")
        {
            Model *m = new Model();
            std::string modelPath = it->path().parent_path().directory_string();
            std::string modelName = it->path().stem();
            std::cout << "Found model: " << modelName << '\n';
            m->loadObj(modelPath.c_str(), modelName.c_str());
            model[modelName] = m;
        }
        ++it;
    }
}
