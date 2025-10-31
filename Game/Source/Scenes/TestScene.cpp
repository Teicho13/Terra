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

}

TestScene::~TestScene()
{
    //Cleanup all objects
}

void TestScene::Update(float DeltaTime)
{
}

//Temp function
glm::vec3 lerp(const glm::vec3& a, const glm::vec3& b, float t) {

    return {
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t,
        a.z + (b.z - a.z) * t
    };
}

//Temp variables
bool ButtonWasPressed = false;
int GridSettings[2] = {2,2};
int tileSpacing = 3;

void TestScene::Render()
{
    Terra::Renderer::RenderScene(m_camera);
    
    int tilesize = Terra::Renderer::s_TileSize;

    glm::mat4 scale = glm::scale(glm::mat4(1.f), glm::vec3(tilesize,tilesize,1.f));

    m_TestTexture.Bind(0);

    //Temp create a grid for testing quads.
    for (int y = 0; y < GridSettings[1]; y++)
    {
        for(int x = 0; x < GridSettings[0]; x++)
        {
            //Temp calculations for  gradiant grid colors
            
            float ty = static_cast<float>(y) / static_cast<float>(GridSettings[1] - 1);
            float tx = static_cast<float>(x) / static_cast<float>(GridSettings[0] - 1);

            float t = (tx + ty) / 2.f;
            glm::vec3 color = lerp(glm::vec3(1.f,0.f,0.f),glm::vec3(0.f,0.f,1.f),t);

            //TODO Replace all transformations with a single transform variable we get from an object.
            glm::mat4 trans(1.f);
            trans = glm::translate(glm::mat4(1.f), glm::vec3(x * (tilesize + tileSpacing) , y * (tilesize + tileSpacing) , 0));
            glm::mat4 model = trans * scale;

            //Alternate between texture and not
            if ((y + x) % 2 == 0)
            {
                //Terra::Renderer::DrawQuad(model, glm::vec4(color, 1.0f));
                Terra::Renderer::DrawQuad(glm::vec3(x * (tilesize + tileSpacing) , y * (tilesize + tileSpacing) , 0),glm::vec3(tilesize,tilesize,1.f), glm::vec4(color, 1.0f));
            }else
            {
                Terra::Renderer::DrawQuad(model, m_TestTexture);
            }
        }
    }

    //Draw all gathered indices.
    Terra::Renderer::Flush();

    m_TestTexture.Unbind();
    
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Window");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("E is pressed: %s", ButtonWasPressed ? "true" : "false");
    ImGui::Text("Camera Position: X: %.2f Y: %.2f Z: %.2f", m_camera.GetPosition().x,m_camera.GetPosition().y,m_camera.GetPosition().z);
    ImGui::Text("Camera ZoomLevel: %.2f", m_camera.GetZoomLevel());
    ImGui::SliderInt("Tile Size", &Terra::Renderer::s_TileSize , 1,100);
    ImGui::SliderInt("Tile Spacing", &tileSpacing , 0,100);
    ImGui::SliderInt2("Grid Settings", &GridSettings[0] , 0,200);
    ImGui::Text("Quads: %d", Terra::Renderer::s_DrawnQuads);
    ImGui::Text("Draw Calls: %d", Terra::Renderer::s_DrawCalls);
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
        m_camera.SetZoomLevel(zoom - 0.1f);
    }

    if (key == GLFW_KEY_MINUS)
    {
        float zoom = m_camera.GetZoomLevel();
        m_camera.SetZoomLevel(zoom + 0.1f);
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
