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

    static void GLFWWindowResizeCallback(GLFWwindow* window, const GLsizei width, const GLsizei height)
    {
        //Make sure we have the correct window size.
        glViewport(0, 0, width, height);
    }

    static void GLFWKeyCallback(GLFWwindow* window, const int key, const int scancode, const int action, const int mods)
    {
        for (const std::unique_ptr<Scene>& CurrentScene : Application::GetApplication()->GetScenes())
        {
            switch (action)
            {
                case GLFW_PRESS:
                    CurrentScene->OnInputPressed(key,scancode, mods);
                break;
                
                case GLFW_REPEAT:
                    CurrentScene->OnInputHeld(key,scancode, mods);
                break;
                
                case GLFW_RELEASE:
                    CurrentScene->OnInputReleased(key,scancode, mods);
                break;

                default:
                break;
            }
            
        }
    }

    static void GLFWMouseButtonCallback(GLFWwindow* window, const int button, const int action, const int mods)
    {
        for (const std::unique_ptr<Scene>& CurrentScene : Application::GetApplication()->GetScenes())
        {
            if (action == GLFW_PRESS)
            {
               CurrentScene->OnMouseClicked(button, mods);
            }

            if (action == GLFW_RELEASE)
            {
               CurrentScene->OnMouseReleased(button, mods);
            }
        }
        
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

        //Setup callback events
        glfwSetWindowSizeCallback(m_Window->GetWindow(), GLFWWindowResizeCallback);
        glfwSetKeyCallback(m_Window->GetWindow(), GLFWKeyCallback);
        glfwSetMouseButtonCallback(m_Window->GetWindow(), GLFWMouseButtonCallback);

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

    std::vector<std::unique_ptr<Scene>>& Application::GetScenes()
    {
        return m_Scenes;
    }

    glm::vec2 Application::GetWindowBuffer() const
    {
        return m_Window->GetWindowBuffer();
    }

    Application* Application::GetApplication()
    {
        assert(s_Application);
        return s_Application;
    }

    float Application::GetTime()
    {
        return static_cast<float>(glfwGetTime());
    }
}
