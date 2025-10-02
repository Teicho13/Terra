#include "Application.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace Terra
{
    static void GLFWErrorCallback(int error, const char* description)
    {
        std::cerr << "GLFW Error: " << description << "\n";
    }
    
    Application::Application()
    {
        glfwSetErrorCallback(GLFWErrorCallback);
        glfwInit();
        
        m_Window = std::make_shared<Window>();
        m_Window->Create();
    }

    Application::~Application()
    {
        m_Window->Destroy();
        glfwTerminate();
    }

    void Application::Run()
    {
        while (!m_Window->ShouldClose())
        {
            glfwPollEvents();

            
            glClearColor(1.f, 0.f, 0.f, 255.f);
            glClear(GL_COLOR_BUFFER_BIT);

            m_Window->Update();
        }
    }
}