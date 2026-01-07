#pragma once

#include "GL_common.h"
#include "Scene.h"
#include "Controls.h"
#include <vector>
#include <memory>

class Application
{
private:
    GLFWwindow *window;
    int window_width;
    int window_height;

    Controls controls;

    Scene *scene = nullptr;

public:
    Application(int window_width, int window_height);
    ~Application();

    bool init();
    void run();
    void cleanup();

    GLFWwindow *getWindow() const;

    // Takes a list of function pointers to scene factories
    void switching_scene(const std::vector<Scene *(*)()> &scenes);
};