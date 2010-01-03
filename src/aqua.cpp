#include <GL/glut.h>
#include "model.hpp"

int main()
{
    Model fish;
    fish.loadObj("clownfish");
    return 0;
}
