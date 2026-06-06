#define GLM_ENABLE_EXPERIMENTAL

#include "World.h"
#include "Core/Application.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Core/Rendering/Renderer.h"

static Terra::WindowSpecs GetWindowSpecs()
{
    return Terra::Application::GetApplication()->GetWindow()->GetWindowSpecs();
}

World::World()
{
}

World::~World()
{
    //Cleanup all objects
}

void World::Update(float DeltaTime)
{
}

void World::Render()
{
    //Draw all gathered indices.

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
