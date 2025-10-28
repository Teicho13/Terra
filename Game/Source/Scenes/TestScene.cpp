#include "TestScene.h"
#include "Core/Rendering/Shader.h"
#include <stb_image.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Core/Application.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

#include "Core/Rendering/Buffer.h"
#include "Core/Rendering/Renderer.h"

//Vertices coordinates square
GLfloat vertices[] =
{
    -50.0f, -50.0f, 0.f,      0.f, 0.f,
    -50.0f,  50.0f, 0.f,      0.f, 1.f,
     50.0f,  50.0f, 0.f,      1.f, 1.f,
     50.0f, -50.0f, 0.0f,     1.f, 0.f
};

//Order in which the vertices will be drawn (optimization)
GLuint Indices[] =
{
    0, 2, 1, 
    0, 3, 2
};

TestScene::TestScene()
    : m_TestTexture(std::filesystem::path("E:/GameDev/Personal/Other/Terra/Terra/Resources/Textures/boomkin.jpg"))
{
    //Create shader using default vertex and fragment (currently hardcoded location)
    std::filesystem::path VertexPath("E:/GameDev/Personal/Other/Terra/Terra/Resources/Shaders/DefaultVertex.vert");
    std::filesystem::path FragmentPath("E:/GameDev/Personal/Other/Terra/Terra/Resources/Shaders/DefaultFragment.frag");
    m_TestShader = Terra::Shader::CreateShader(VertexPath,FragmentPath);

    //Bind Vertex Array so that the next couple items apply to this.
    m_VAO.Bind();

    //Bind vertex buffer and link it to the vertices.
    Terra::VertexBuffer vb(vertices, sizeof(vertices));
    
    //Bind elements buffer and link it to the indeces.
    Terra::IndexBuffer ib(Indices, 6);
    
    //Link VBO attributes (Coordinates & Texture coordinate)
    m_VAO.AddAttribute(vb,{.count = 3, .type = GL_FLOAT, .normalized = GL_FALSE, .stride = 5 * sizeof(float), .offset = (void*)0});
    m_VAO.AddAttribute(vb,{.count = 2, .type = GL_FLOAT, .normalized = GL_FALSE, .stride = 5 * sizeof(float), .offset = (void*)(3 * sizeof(float))});
    
    //Unbind all objects (order matters)
    m_VAO.Unbind();
    vb.Unbind();
    ib.Unbind();
    
    //Get shader texture uniform variable location (Tex 0 - 16).
    GLint textureUniform = glGetUniformLocation(m_TestShader, "tex0");
    glUseProgram(m_TestShader);
    glUniform1i(textureUniform, 0);
    
    m_MatrixUniformID = glGetUniformLocation(m_TestShader, "projectionViewMatrix");
}

TestScene::~TestScene()
{
    //Cleanup all objects
    glDeleteProgram(m_TestShader);
}

void TestScene::Update(float DeltaTime)
{
}

//Temp variables
bool ButtonWasPressed = false;
glm::vec3 ModelTranslationA(50.0f, 50.0f, 0.0f);
glm::vec3 ModelTranslationB(150.0f, 150.0f, 0.0f);

void TestScene::Render()
{
    glUseProgram(m_TestShader);
    
    //Set vertex uniform values
    const glm::mat4 ModelA = glm::translate(glm::mat4(1.f), ModelTranslationA);
    const glm::mat4 MVPA = m_camera.GetProjectionViewMatrix() * ModelA;
    glUniformMatrix4fv(m_MatrixUniformID, 1, GL_FALSE, glm::value_ptr(MVPA));

    Terra::Renderer::Draw(m_VAO,m_TestTexture,m_TestShader);

    const glm::mat4 ModelB = glm::translate(glm::mat4(1.f), ModelTranslationB);
    const glm::mat4 MVPB = m_camera.GetProjectionViewMatrix() * ModelB;
    glUniformMatrix4fv(m_MatrixUniformID, 1, GL_FALSE, glm::value_ptr(MVPB));

    Terra::Renderer::Draw(m_VAO,m_TestTexture,m_TestShader);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Window");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("E is pressed: %s", ButtonWasPressed ? "true" : "false");
    ImGui::Text("Camera Position: X: %.2f Y: %.2f Z: %.2f", m_camera.GetPosition().x,m_camera.GetPosition().y,m_camera.GetPosition().z);
    ImGui::Text("Camera ZoomLevel: %.2f", m_camera.GetZoomLevel());
    ImGui::SliderFloat3("Model A Translation", &ModelTranslationA.x , 0.f,1280.f);
    ImGui::SliderFloat3("Model B Translation", &ModelTranslationB.x , 0.f,1280.f);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void TestScene::OnInputPressed(int key, int scancode, int mods)
{
    if (key == GLFW_KEY_E)
    {
        ButtonWasPressed = true;
    }

    if (key == GLFW_KEY_A)
    {
        auto newPos = m_camera.GetPosition();
        newPos.x -= 1.f;
        m_camera.SetPosition(newPos);
    }

    if (key == GLFW_KEY_D)
    {
        auto newPos = m_camera.GetPosition();
        newPos.x += 1.f;
        m_camera.SetPosition(newPos);
    }

    if (key == GLFW_KEY_EQUAL)
    {
        float zoom = m_camera.GetZoomLevel();
        m_camera.SetZoomLevel(zoom + 0.1f);
    }

    if (key == GLFW_KEY_MINUS)
    {
        float zoom = m_camera.GetZoomLevel();
        m_camera.SetZoomLevel(zoom - 0.1f);
    }
}

void TestScene::OnInputReleased(int key, int scancode, int mods)
{
    if (key == GLFW_KEY_E)
    {
        ButtonWasPressed = false;
    }
}

void TestScene::OnInputHeld(int key, int scancode, int mods)
{
    if (key == GLFW_KEY_A)
    {
        auto newPos = m_camera.GetPosition();
        newPos.x -= 1.f;
        m_camera.SetPosition(newPos);
    }

    if (key == GLFW_KEY_D)
    {
        auto newPos = m_camera.GetPosition();
        newPos.x += 1.0f;
        m_camera.SetPosition(newPos);
    }
}
