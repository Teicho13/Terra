#include "Application.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <filesystem>
#include "Rendering/Shader.h"

namespace Terra
{
    //Vertices coordinates triangle
    GLfloat vertices[] =
    {
        0.f, 1.f, 0.f,
        -1.f, -1.f, 0.f,
        1.f, -1.f, 0.f,
    };
    
    //Testing if stb works
    int ImgWidth, ImgHeight, numColCh;
    unsigned char* bytes = stbi_load("test.png", &ImgWidth, &ImgHeight, &numColCh, 0);
    
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
        m_IsRunning = true;
        
        std::filesystem::path VertexPath("E:/GameDev/Personal/Other/Terra/Terra/Resources/Shaders/DefaultVertex.vert");
        std::filesystem::path FragmentPath("E:/GameDev/Personal/Other/Terra/Terra/Resources/Shaders/DefaultFragment.frag");
        GLuint testShader = Shader::CreateShader(VertexPath,FragmentPath);

        GLuint VBO;
        GLuint VAO;
        
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        
        while (m_IsRunning)
        {
            glfwPollEvents();

            if (m_Window->ShouldClose())
            {
                m_IsRunning = false;
                break;
            }

            const auto WindowBuffer = m_Window->GetWindowBuffer();
            glViewport(0, 0, static_cast<GLsizei>(WindowBuffer.x), static_cast<GLsizei>(WindowBuffer.y));
            
            glClearColor(0.f, 0.1f, 0.2f, 255.f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            glUseProgram(testShader);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            m_Window->Update();
        }

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(testShader);
    }
}
