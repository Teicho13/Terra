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

#include "Core/Rendering/Renderer.h"
#include "Core/FileIO.h"


TestScene::TestScene()
    : m_AnimatedSpriteTest(Terra::FileIO::GetEngineFile("Textures\\ExampleSpriteSheet.png"),3)
{
    m_AnimatedSpriteTest.GetAnimation().SetLooped(true);
    m_AnimatedSpriteTest.GetAnimation().SetFrameSpeed(5.f);
    m_AnimatedSpriteTest.GetAnimation().Play();
    m_AnimatedSpriteTest.SetPosition({300.f,200.f,1.f});

    m_TestMap.SetMapData(Terra::FileIO::GetGameFile("Tilemaps\\Data\\FirstMap.csv"));
    m_TestMap.SetMapTexture(Terra::FileIO::GetGameFile("Tilemaps\\GeneralTiles.png"));
    
}

TestScene::~TestScene()
{
    //Cleanup all objects
}

void TestScene::Update(float DeltaTime)
{
    m_AnimatedSpriteTest.Update(DeltaTime);
    m_AnimatedSpriteTest.SetScale(glm::vec3(32.f,32.f,1.f));
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

void TestScene::Render()
{
    Terra::Renderer::RenderScene(m_camera);
    
    m_TestMap.DrawMap();
    m_AnimatedSpriteTest.Draw();

    //Draw all gathered indices.
    Terra::Renderer::Flush();
    
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Window");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("E is pressed: %s", ButtonWasPressed ? "true" : "false");
    ImGui::Text("Camera Position: X: %.2f Y: %.2f Z: %.2f", m_camera.GetPosition().x,m_camera.GetPosition().y,m_camera.GetPosition().z);
    ImGui::Text("Camera ZoomLevel: %.2f", m_camera.GetZoomLevel());
    ImGui::Text("Quads: %d", Terra::RenderStats::s_DrawnQuads);
    ImGui::Text("Textures used: %d", Terra::RenderStats::s_DrawnTextures);
    ImGui::Text("Draw Calls: %d", Terra::RenderStats::s_DrawCalls);
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
