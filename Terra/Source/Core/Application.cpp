#include "Application.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <filesystem>

#include "DeltaTime.h"
#include "Rendering/Shader.h"

namespace Terra
{
    //Vertices coordinates triangle
    GLfloat vertices[] =
    {
        -0.5f, -0.5f, 0.f,      0.f, 0.f,
        -0.5f,  0.5f, 0.f,      0.f, 1.f,
         0.5f,  0.5f, 0.f,      1.f, 1.f,
         0.5f, -0.5f, 0.0f,     1.f, 0.f
    };

    GLuint Indices[] =
    {
        0, 2, 1, 
        0, 3, 2
    };
    
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
        GLuint EBO;
        
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1,&EBO);
        
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // Gets ID of uniform called "scale"
        GLuint uniID = glGetUniformLocation(testShader, "scale");


        stbi_set_flip_vertically_on_load(true);
        
        //Testing if stb works
        int ImgWidth, ImgHeight, numColCh;
        unsigned char* bytes = stbi_load("E:/GameDev/Personal/Other/Terra/Terra/Resources/Textures/boomkin.jpg", &ImgWidth, &ImgHeight, &numColCh, 0);

        GLuint TextureID;
        glGenTextures(1,&TextureID);
        glBindTexture(GL_TEXTURE_2D, TextureID);
        
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,ImgWidth,ImgHeight,0,GL_RGB,GL_UNSIGNED_BYTE,bytes);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(bytes);
        
        GLint textureUniform = glGetUniformLocation(testShader, "tex0");
        glUseProgram(testShader);
        glUniform1i(textureUniform, 0);
        
        DeltaTime deltaTime(120.0);
        
        while (m_IsRunning)
        {
            deltaTime.UpdateTime();
            glfwPollEvents();

            if (m_Window->ShouldClose())
            {
                m_IsRunning = false;
                break;
            }
            
            while (deltaTime.ShouldUpdate())
            {
                deltaTime.ReduceAccumulation();
            }
            
            const auto WindowBuffer = m_Window->GetWindowBuffer();
            glViewport(0, 0, static_cast<GLsizei>(WindowBuffer.x), static_cast<GLsizei>(WindowBuffer.y));
            
            glClearColor(0.f, 0.1f, 0.2f, 255.f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            glUseProgram(testShader);

            //Set vertex uniform values
            glBindTexture(GL_TEXTURE_2D, TextureID);
            glBindVertexArray(VAO);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            
            m_Window->Update();
        }

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(testShader);
    }
}
