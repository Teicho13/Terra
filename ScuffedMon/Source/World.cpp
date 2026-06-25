#include "World.h"
#include "Core/Application.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Core/Rendering/Renderer.h"

#include "Player.h"
#include "Core/FileIO.h"

static Terra::WindowSpecs GetWindowSpecs()
{
    return Terra::Application::GetApplication()->GetWindow()->GetWindowSpecs();
}

World::World() : m_CameraManager(static_cast<float>(GetWindowSpecs().width),static_cast<float>(GetWindowSpecs().height))
{
    m_Player = std::make_unique<Player>(Terra::FileIO::GetScuffedMonFile(R"(Characters\Player\player-idle.png)"));
}

World::~World()
{
    //Cleanup all objects
}

void World::Update(float DeltaTime)
{
    m_CameraManager.Update(DeltaTime);
    m_Player->Update(DeltaTime);
}

void World::Render()
{
    Terra::Renderer::RenderScene(m_CameraManager.GetCamera());
    //Draw all gathered indices.
    m_Player->Draw();
    

    Terra::Renderer::Flush();
    

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Window");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("Quads: %d", Terra::RenderStats::s_DrawnQuads);
    ImGui::Text("Textures used: %d", Terra::RenderStats::s_DrawnTextures);
    ImGui::Text("Draw Calls: %d", Terra::RenderStats::s_DrawCalls);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
