#include "Application.h"
#include <iostream>
#include <ctime>
#include "Scene.h"

Application::Application(int window_width, int window_height)
    : window(nullptr), window_width(window_width), window_height(window_height)
{
}

Application::~Application()
{
    cleanup();
}

bool Application::init()
{
    // Initialize Random Seed
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    if (!glfwInit())
    {
        std::cerr << "❌ ERROR: could not start GLFW3" << std::endl;
        return false;
    }

    // OpenGL Version 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(window_width, window_height, "ZPG Engine", NULL, NULL);
    if (!window)
    {
        std::cerr << "❌ ERROR: could not open window with GLFW3" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Start GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "❌ ERROR: could not start GLEW" << std::endl;
        return false;
    }

    // OpenGL State Defaults
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    // Debug Info
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Renderer:       " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Vendor:         " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "GLSL Version:   " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // Attach controls
    controls.attach_to_window(window);

    return true;
}

void Application::run()
{
    scene = login_scene();

    float last_time = (float)glfwGetTime();
    float current_time = 0;
    float delta_time = 0;

    // Available Scenes
    std::vector<Scene *(*)()> available_scenes = {
        login_scene,
        sphere_scene,
        whack_the_mole,
        forest_scene,
        solar_system_scene};

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        current_time = (float)glfwGetTime();
        delta_time = current_time - last_time;
        last_time = current_time;

        // Scene logic
        switching_scene(available_scenes);

        if (scene)
        {
            scene->process(window, delta_time, &controls);
        }

        Controls::end_frame();
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

void Application::switching_scene(const std::vector<Scene *(*)()> &scenes)
{
    for (size_t i = 0; i < scenes.size(); i++)
    {
        if (controls.is_key_pressed(GLFW_KEY_1 + (int)i))
        {
            if (scene && scene->get_scene_id() != (int)i)
            {
                std::cout << "🔄 Switching to scene ID: " << i << std::endl;

                delete scene;
                scene = nullptr;

                scene = scenes[i]();

                glfwSetTime(glfwGetTime());
            }
        }
    }
}

void Application::cleanup()
{
    if (scene)
    {
        delete scene;
        scene = nullptr;
    }

    if (window)
    {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
}

GLFWwindow *Application::getWindow() const
{
    return window;
}