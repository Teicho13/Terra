#define GLM_ENABLE_EXPERIMENTAL

#include "TestScene.h"
#include "Core/Rendering/Shader.h"
#include <stb_image.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/string_cast.hpp>

#include "Core/Application.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <array>

#include "Core/Rendering/Buffer.h"
#include "Core/Rendering/Renderer.h"


//pre-calculations for amount of vertices and indices based on the quads
constexpr size_t QuadCount = 1000;
constexpr size_t VertexCount = QuadCount * 4;
constexpr size_t IndexCount = QuadCount * 6;

struct VertexData
{
    std::array<float, 3> Position;
    std::array<float, 4> Color;
    std::array<float, 2> Texcoord;
};


TestScene::TestScene()
    : m_TestTexture(std::filesystem::path("E:/GameDev/Personal/Other/Terra/Terra/Resources/Textures/boomkin.jpg"))
{
    //Create shader using default vertex and fragment (currently hardcoded location)
    std::filesystem::path VertexPath("E:/GameDev/Personal/Other/Terra/Terra/Resources/Shaders/DefaultVertex.vert");
    std::filesystem::path FragmentPath("E:/GameDev/Personal/Other/Terra/Terra/Resources/Shaders/DefaultFragment.frag");
    m_Shader = new Terra::Shader(VertexPath,FragmentPath);

    //Bind Vertex Array so that the next couple items apply to this.
    m_VAO.Bind();

    //Bind vertex buffer and link it to the vertices.
    m_VertexBuffer = new Terra::VertexBuffer(VertexCount * sizeof(VertexData));
    
    //Link VBO attributes (Coordinates & Texture coordinate)
    m_VAO.AddAttribute(*m_VertexBuffer,{.count = 3, .type = GL_FLOAT, .normalized = GL_FALSE, .stride = sizeof(VertexData), .offset = (void*)offsetof(VertexData, Position)});
    m_VAO.AddAttribute(*m_VertexBuffer,{.count = 4, .type = GL_FLOAT, .normalized = GL_FALSE, .stride = sizeof(VertexData), .offset = (void*)offsetof(VertexData, Color)});
    m_VAO.AddAttribute(*m_VertexBuffer,{.count = 2, .type = GL_FLOAT, .normalized = GL_FALSE, .stride = sizeof(VertexData), .offset = (void*)offsetof(VertexData, Texcoord)});

    //Generate indices based on the amount of quads we want.
    uint32_t Indices[IndexCount];
    uint32_t offset = 0;
    for (size_t i = 0; i < IndexCount; i += 6)
    {
        Indices[i + 0] = 0 + offset;
        Indices[i + 1] = 2 + offset;
        Indices[i + 2] = 1 + offset;
        
        Indices[i + 3] = 0 + offset;
        Indices[i + 4] = 3 + offset;
        Indices[i + 5] = 2 + offset;

        offset += 4;
    }
    
    //Bind elements buffer and link it to the indeces.
    Terra::IndexBuffer ib(Indices, IndexCount);
    
    //Unbind all objects (order matters)
    m_VAO.Unbind();
    m_VertexBuffer->Unbind();
    ib.Unbind();

    m_Shader->Bind();
    m_Shader->SetInt("u_Texture", 0);
}

TestScene::~TestScene()
{
    //Cleanup all objects
    delete m_Shader;
}

void TestScene::Update(float DeltaTime)
{
}

//Temp variables
bool ButtonWasPressed = false;
glm::vec3 ModelTranslationA(50.0f, 50.0f, 0.0f);
glm::vec3 ModelTranslationB(150.0f, 150.0f, 0.0f);

static VertexData* CreateQuad(VertexData* target, float x, float y)
{
    float size = 100;
    
    //Bottom left
    target->Position = {x, y, 0.0f};
    target->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
    target->Texcoord = { 0.0f, 0.0f };
    target++;

    //Top Left
    target->Position = {x, y + size, 0.0f};
    target->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
    target->Texcoord = { 0.0f, 1.0f };
    target++;
    
    //Top Right
    target->Position = {x + size, y + size, 0.0f};
    target->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
    target->Texcoord = { 1.0f, 1.0f };
    target++;

    //Bottom Right
    target->Position = {x + size, y, 0.0f};
    target->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
    target->Texcoord = { 1.0f, 0.0f };
    target++;

    return target;
}

void TestScene::Render()
{
    m_Shader->Bind();

    int indexCount = 0;
    std::array<VertexData, VertexCount> Vertices;
    VertexData* buffer = Vertices.data();
    
    for (size_t y = 0; y < 8; y++)
    {
        for (size_t x = 0; x < 13; x++)
        {
            buffer = CreateQuad(buffer,x * 100,y * 100);
            indexCount += 6;
        }
    }
    
    buffer = CreateQuad(buffer,ModelTranslationA.x, ModelTranslationA.y);
    indexCount += 6;
    
    m_VAO.Bind();
    m_VertexBuffer->Bind();
    glBufferSubData(GL_ARRAY_BUFFER,0,Vertices.size() * sizeof(VertexData),Vertices.data());
    
    //Set vertex uniform values
    const glm::mat4 MVP = m_camera.GetProjectionViewMatrix();
    m_Shader->SetMat4("u_ProjectionViewMatrix", MVP);
    
    Terra::Renderer::Draw(m_VAO,m_TestTexture,*m_Shader,indexCount);
    
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
