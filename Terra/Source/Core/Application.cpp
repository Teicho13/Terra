#include "Application.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>


namespace Terra
{
    Application::Application()
    { 

    }

    Application::~Application()
    {

    }

    void Application::Run()
    {
        if (!glfwInit())
        {
            glfwTerminate();
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        GLFWwindow* window = glfwCreateWindow(800, 600, "test", NULL, NULL);

        if (!window)
        {
            glfwTerminate();
        }

        int bufferWidth, bufferHeight;
        glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

        glfwMakeContextCurrent(window);


        glewExperimental = GL_TRUE;

        if (glewInit() != GLEW_OK)
        {
            glfwDestroyWindow(window);
            glfwTerminate();
        }

        glViewport(0, 0, bufferWidth, bufferHeight);

        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();

            glClearColor(1.f, 0.f, 0.f, 255.f);
            glClear(GL_COLOR_BUFFER_BIT);

            glfwSwapBuffers(window);
        }

        glfwDestroyWindow(window);
        glfwTerminate();
    }
}