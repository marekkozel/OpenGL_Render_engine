// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include "Application.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
    Application app(1920, 1080);
    if (!app.init())
    {
        return -1;
    }
    app.run();
    app.cleanup();
    return 0;
}