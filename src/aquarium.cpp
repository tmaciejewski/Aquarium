//      aquarium.cpp
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


#include "aquarium.hpp"

Aquarium::Aquarium()
{
    srand(time(0));
}

Aquarium::~Aquarium()
{
    for(std::vector<Fish*>::iterator it = fish.begin();
        it != fish.end(); ++it)
    {
        delete *it;
    }
}

void Aquarium::display() const
{
    for(std::vector<Fish*>::const_iterator it = fish.begin();
        it != fish.end(); ++it)
    {
        (*it)->display();
    }
}

void Aquarium::update()
{
    for(std::vector<Fish*>::iterator it = fish.begin();
        it != fish.end(); ++it)
    {
        (*it)->update();
    }
}

void Aquarium::addFish(const Model *model, GLfloat scale)
{
    Fish *f = new Fish(model, scale);
    addFish(f);
}

void Aquarium::addFish(Fish *f)
{
    fish.push_back(f);
}
