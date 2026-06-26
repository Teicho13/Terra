#include "Application.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include <filesystem>
#include <stb_image.h>
#include <glad/glad.h>

#include "DeltaTime.h"
#include "Events/Input.h"
#include "Events/Interfaces/InputListener.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Rendering/Renderer.h"

namespace Terra
{
    static Application* s_Application;
    
    static void GLFWErrorCallback(int error, const char* description)
    {
        std::cerr << "GLFW Error: " << description << "\n";
    }

    static void GLFWMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        for (const std::unique_ptr<Scene>& CurrentScene : Application::GetApplication()->GetScenes())
        {
            if (auto* inputListener = dynamic_cast<InputListener*>(CurrentScene.get()))
            {
                inputListener->OnMouseScroll(xoffset, yoffset);
            }
        }
    }

    static void GLFWWindowResizeCallback(GLFWwindow* window, const GLsizei width, const GLsizei height)
    {
        //Make sure we have the correct window size.
        glViewport(0, 0, width, height);

        if (!Application::GetApplication()->GetScenes().empty())
        {
            const auto& CurrentScene = Application::GetApplication()->GetScenes()[0];
            if (auto* inputListener = dynamic_cast<InputListener*>(CurrentScene.get()))
            {
                inputListener->OnScreenResize(static_cast<float>(width), static_cast<float>(height)); 
            }
            
        }
    }

    static void GLFWKeyCallback(GLFWwindow* window, const int key, const int scancode, const int action, const int mods)
    {
        Input::OnKey(key,action, mods);
        for (const std::unique_ptr<Scene>& CurrentScene : Application::GetApplication()->GetScenes())
        {
            if (auto* inputListener = dynamic_cast<InputListener*>(CurrentScene.get()))
            {
                switch (action)
                {
                case GLFW_PRESS:
                    inputListener->OnInputPressed(key,scancode, mods);
                    break;
                
                case GLFW_REPEAT:
                    inputListener->OnInputHeld(key, scancode, mods);
                    break;
                
                case GLFW_RELEASE:
                    inputListener->OnInputReleased(key, scancode, mods);
                    break;

                default:
                    break;
                }
            }
        }
    }

    static void GLFWMouseButtonCallback(GLFWwindow* window, const int button, const int action, const int mods)
    {
        double xpos, ypos;
        glfwGetCursorPos(window,&xpos,&ypos);
        Input::OnMouseClick(button,action,static_cast<float>(xpos),static_cast<float>(ypos));
        for (const std::unique_ptr<Scene>& CurrentScene : Application::GetApplication()->GetScenes())
        {
            if (auto* inputListener = dynamic_cast<InputListener*>(CurrentScene.get()))
            {
                if (action == GLFW_PRESS)
                {
                    inputListener->OnMouseClicked(button, mods,static_cast<float>(xpos),static_cast<float>(ypos));
                }

                if (action == GLFW_RELEASE)
                {
                    inputListener->OnMouseReleased(button, mods,static_cast<float>(xpos),static_cast<float>(ypos));
                }
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
        glfwSetScrollCallback(m_Window->GetWindow(), GLFWMouseScrollCallback);

        Renderer::Initialize();

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

        m_Scenes.clear();
        Renderer::Shutdown();
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

            Input::BeginInputFrame();
            glfwPollEvents();

            //Early exit out if we closed window
            if (m_Window->ShouldClose())
            {
                m_IsRunning = false;
                break;
            }

            Renderer::Clear();

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
            for (const std::unique_ptr<Scene>& CurrentScene : m_Scenes)
            {
                CurrentScene->Render();
            }

            m_Window->Update();
        }
    }

    void Application::RequestEnd()
    {
        m_IsRunning = false;
    }

    std::vector<std::unique_ptr<Scene>>& Application::GetScenes()
    {
        return m_Scenes;
    }

    std::shared_ptr<Window>& Application::GetWindow()
    {
        return m_Window;
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
