#include "Application.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include <filesystem>
#include <stb_image.h>
#include <glad/glad.h>

#include "DeltaTime.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Terra
{
    static void GLFWErrorCallback(int error, const char* description)
    {
        std::cerr << "GLFW Error: " << description << "\n";
    }
    
    Application::Application()
    {
        s_Application = this;

        //Set error callback function and initialize glfw
        glfwSetErrorCallback(GLFWErrorCallback);
        glfwInit();

        //Flip all stb images
        stbi_set_flip_vertically_on_load(true);
        
        //Create main window
        m_Window = std::make_shared<Window>();
        m_Window->Create();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(m_Window->GetWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    Application::~Application()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        m_Window->Destroy();
        glfwTerminate();
    }

    void Application::Run()
    {
        m_IsRunning = true;
        
        //DeltaTime object with target FPS
        DeltaTime deltaTime(120.0);
        
        while (m_IsRunning)
        {
            //Update current time and calculate new delta time
            deltaTime.UpdateTime();
            
            glfwPollEvents();

            //Early exit out if we closed window
            if (m_Window->ShouldClose())
            {
                m_IsRunning = false;
                break;
            }

            //In the case we fall behind, keep updating until we are caught up otherwise we update on a fixed time.
            while (deltaTime.ShouldUpdate())
            {
                for (const std::unique_ptr<Scene>& CurrentScene : m_Scenes)
                {
                    CurrentScene->Update(deltaTime.Get());
                }
                
                deltaTime.ReduceAccumulation();
            }

            //Rendering does not follow fixed timing

            //Clear screen
            glClearColor(0.f, 0.1f, 0.2f, 255.f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            for (const std::unique_ptr<Scene>& CurrentScene : m_Scenes)
            {
                CurrentScene->Render();
            }

            m_Window->Update();
        }
    }

    glm::vec2 Application::GetWindowBuffer() const
    {
        return m_Window->GetWindowBuffer();
    }

    Application* Application::Get()
    {
        assert(s_Application);
        return s_Application;
    }

    float Application::GetTime()
    {
        return static_cast<float>(glfwGetTime());
    }
}
