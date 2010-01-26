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

Aquarium::Aquarium(GLuint width, GLuint height, GLuint depth)
{
    srand(time(0));
    size[0] = width;
    size[1] = height;
    size[2] = depth;
}

Aquarium::~Aquarium()
{
    for(std::vector<Fish*>::iterator it = fish.begin();
        it != fish.end(); ++it)
    {
        delete *it;
    }

    glDeleteTextures(3, texture);
}

void Aquarium::textureFromImg(GLuint t, const char *filename)
{
    SDL_Surface *img = IMG_Load(filename);
    if (img)
    {
        glBindTexture(GL_TEXTURE_2D, t);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w,
                    img->h, 0, GL_RGB, GL_UNSIGNED_BYTE,
                    img->pixels);
        SDL_FreeSurface(img);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Aquarium::init()
{
    initShader();

    glGenTextures(3, texture);

    textureFromImg(texture[0], DATADIR "/wall.jpg");
    textureFromImg(texture[1], DATADIR "/sand.jpg");
    textureFromImg(texture[2], DATADIR "/water.jpg");
}

void Aquarium::display() const
{
    static GLfloat t = 0.0;

    displayAquarium();

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

void Aquarium::displaySquare(GLfloat width, GLfloat height) const
{
    GLfloat u = 1.0, v = 1.0;
    glScalef(width / 2.0, height / 2.0, 1.0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, v); glVertex3f(1.0, -1.0, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(1.0, 1.0, 0.0);
    glTexCoord2f(u, 0.0); glVertex3f(-1.0, 1.0, 0.0);
    glTexCoord2f(u, v); glVertex3f(-1.0, -1.0, 0.0);
    glEnd();
}

void Aquarium::displayAquarium() const
{
    GLfloat matrix[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    glTranslatef(0.0, 0.0, -size[2] / 2.0);
    displaySquare(size[0], size[1]);

    glLoadMatrixf(matrix);
    glTranslatef(-size[0] / 2.0, 0.0, 0.0);
    glRotatef(90.0, 0.0, 1.0, 0.0);
    displaySquare(size[2], size[1]);

    glLoadMatrixf(matrix);
    glTranslatef(size[0] / 2.0, 0.0, 0.0);
    glRotatef(-90.0, 0.0, 1.0, 0.0);
    displaySquare(size[2], size[1]);

    glLoadMatrixf(matrix);
    glTranslatef(0.0, 0.0, size[2] / 2.0);
    glRotatef(180.0, 0.0, 1.0, 0.0);
    displaySquare(size[0], size[1]);

    glLoadMatrixf(matrix);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTranslatef(0.0, -size[1] / 2.0, 0.0);
    glRotatef(-90, 1.0, 0.0, 0.0);
    displaySquare(size[0], size[2]);

    glLoadMatrixf(matrix);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTranslatef(0.0, size[1] / 2.0, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    displaySquare(size[0], size[2]);

    glPopMatrix();
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
        {
            (*it)->setState(Fish::S_MOVING);
        }
    }
}

bool Aquarium::collides(const Fish *f) const
{
    if (f && f->getModel())
    {
        const GLfloat *bbox = f->getModel()->getBBox(), *fpos = f->getXYZ();
        GLfloat scale = f->getScale();

        for (int i = 0; i < 8; ++i)
        {

            GLfloat x = fpos[0] + scale * (bbox[3 * i] - f->getModel()->getCenter(0)),
                    y = fpos[1] + scale * (bbox[3 * i + 1] - f->getModel()->getCenter(1)),
                    z = fpos[2] + scale * (bbox[3 * i + 2] - f->getModel()->getCenter(2));
            if (x >= size[0] / 2.0 || x <= -size[0] / 2.0 ||
                y >= size[1] / 2.0 || y <= -size[1] / 2.0 ||
                z >= size[2] / 2.0 || z <= -size[2] / 2.0)
            {
                std::cout << "Out of the aquarium\n";
                return true;
            }
        }

        for(std::vector<Fish*>::const_iterator it = fish.begin();
            it != fish.end(); ++it)
        {
            if (*it != f && f->collides(*it))
                return true;
        }
    }

    return false;
}

bool Aquarium::addFish(const Model *model, GLfloat scale)
{
    GLfloat x, y, z;
    Fish *f = new Fish(model, scale);
    unsigned i = 0;
    do {
        x = size[0] * ((rand() / (float) RAND_MAX) - 0.5);
        z = size[2] * ((rand() / (float) RAND_MAX) - 0.5);
        y = size[1] * ((rand() / (float) RAND_MAX) - 0.5);
        f->setXYZ(x, y, z);
        ++i;
    } while (collides(f) && i < 100);

    if (i < 100)
    {
        addFish(f);
        return true;
    }
    else
        return false;
}

void Aquarium::addFish(Fish *f)
{
    fish.push_back(f);
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
                      "varying vec3 normal;"
                      "void main(void)\n"
                      "{\n"
                        "normal = gl_NormalMatrix * gl_Normal;\n"
                        "vec4 v = vec4(gl_Vertex);\n"
                        "v.z = v.z + 0.02 * cos(15.0 * v.x + 2.0 * time);\n"
                        "gl_Position = gl_ModelViewProjectionMatrix * v;\n"
                        "gl_TexCoord[0] = gl_MultiTexCoord0;\n"
                      "}"};

    const GLchar *fsSource[] = {"uniform sampler2D tex;\n"
                                 "varying vec3 normal;\n"
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
