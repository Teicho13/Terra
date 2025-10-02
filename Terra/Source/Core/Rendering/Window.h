#pragma once
#define GLFW_INCLUDE_NONE
#include <string>
#include <GLFW/glfw3.h>
#include <glm.hpp>

namespace Terra
{
    struct WindowSpecs
    {
        int width = 800;
        int height = 600;
        std::string title = "Terra";
        bool VSync = false;
    };
    
    class Window
    {

    public:

        Window(const WindowSpecs& specs = WindowSpecs());
        ~Window();

        void Create();
        void Destroy();
        void Update();
        
        GLFWwindow* GetWindow() const;
        glm::vec2 GetWindowBuffer() const;
        bool ShouldClose() const;

    private:
        GLFWwindow* m_Window = nullptr;
        WindowSpecs m_WindowSpecs;
    };
}


