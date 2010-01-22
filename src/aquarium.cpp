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
    //initShader();
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
    static GLfloat t = 0.0;
    glUseProgram(program);
    glUniform1f(timeLocation, t);
    for(std::vector<Fish*>::const_iterator it = fish.begin();
        it != fish.end(); ++it)
    {
        (*it)->display();
    }

    t += 0.1;
    if (t > 2 * M_PI)
        t -= 2 * M_PI;

    glUseProgram(0);
}

void Aquarium::update()
{

    for(std::vector<Fish*>::iterator it = fish.begin();
        it != fish.end(); ++it)
    {
        (*it)->update();
        if (collides(*it))
        {
            (*it)->setState(Fish::S_COLLISION);
        }
        else
            (*it)->setState(Fish::S_MOVING);
    }
}

bool Aquarium::collides(const Fish *f) const
{
    for(std::vector<Fish*>::const_iterator it = fish.begin();
        it != fish.end(); ++it)
    {
        if (*it != f)
        {
            if (f->collides(*it))
                return true;
        }
    }

    return false;
}

void Aquarium::addFish(const Model *model, GLfloat scale)
{
    Fish *f = new Fish(model, scale);
    GLfloat x, z;
    x = rand() % 50 - 25;
    z = rand() % 50 - 25;
    f->setXYZ(x, 0.0, z);
    addFish(f);
}

void Aquarium::addFish(Fish *f)
{
    fish.push_back(f);
}

void printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog =  new char[infologLength];
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        std::cout << infoLog << std::endl;
        delete [] infoLog;
    }
}

void printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog =  new char[infologLength];
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        std::cout << infoLog << std::endl;
        delete [] infoLog;
    }
}

void Aquarium::initShader()
{
    const GLchar *vsSource[] = {"uniform float time;\n"
                      "void main(void)\n"
                      "{\n"
                        "vec4 v = vec4(gl_Vertex);\n"
                        "v.z = v.z + 0.02 * sin(15.0 * v.x + 2.0 * time);\n"
                        "gl_Position = gl_ModelViewProjectionMatrix * v;\n"
                        "gl_TexCoord[0] = gl_MultiTexCoord0;\n"
                      "}"};

    const GLchar *fsSource[] = {"uniform sampler2D tex;\n"
                                "void main()\n{\n"
                                "gl_FragColor = texture2D(tex,gl_TexCoord[0].st);\n"
                             "}"};

    GLuint vertexShader   = glCreateShader(GL_VERTEX_SHADER),
           fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, vsSource, NULL);
    glShaderSource(fragmentShader, 1, fsSource, NULL);

    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);

    printProgramInfoLog(program);

    glUseProgram(program);
    timeLocation = glGetUniformLocation(program, "time");
    glUseProgram(0);
}
