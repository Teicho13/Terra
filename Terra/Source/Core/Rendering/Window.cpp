#include "Window.h"
#include <cassert>
#include <iostream>
#include <glad/glad.h>

namespace Terra
{
    Window::Window(const WindowSpecs& specs)
        : m_WindowSpecs(specs){}

    Window::~Window()
    {
        Destroy();
    }

    void Window::Create()
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        m_Window = glfwCreateWindow(m_WindowSpecs.width, m_WindowSpecs.height, m_WindowSpecs.title.c_str(), NULL, NULL);

        if (!m_Window)
        {
            std::cout << "Failed to create GLFW window";
            assert(false);
        }
        
        glfwMakeContextCurrent(m_Window);
        
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        glfwSwapInterval(m_WindowSpecs.VSync ? 1 : 0);
    }

    void Window::Destroy()
    {
        if (m_Window)
        {
            glfwDestroyWindow(m_Window);
        }
        m_Window = nullptr;
    }

    void Window::Update()
    {
        glfwSwapBuffers(m_Window);
    }

    GLFWwindow* Window::GetWindow() const
    {
        return m_Window;
    }

    glm::vec2 Window::GetWindowBuffer() const
    {
        int BufferWidth, BufferHeight;
        glfwGetWindowSize(m_Window, &BufferWidth, &BufferHeight);
        return {BufferWidth, BufferHeight};
    }

    bool Window::ShouldClose() const
    {
        return glfwWindowShouldClose(m_Window) != 0;
    }
}
