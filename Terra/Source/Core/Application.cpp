#include "Application.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

namespace Terra
{
    //Temp vertext shader source code
    const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
    //Temp fragment shader source code
    const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

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
        
        GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(VertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(VertexShader);

        GLuint FragShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(FragShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(FragShader);

        GLuint ShaderProgram = glCreateProgram();
        glAttachShader(ShaderProgram, VertexShader);
        glAttachShader(ShaderProgram, FragShader);
        glLinkProgram(ShaderProgram);

        glDeleteShader(VertexShader);
        glDeleteShader(FragShader);

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
            
            glUseProgram(ShaderProgram);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            m_Window->Update();
        }

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(ShaderProgram);
    }
}