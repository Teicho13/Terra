#include "PerlinVisualizerScene.h"

#include "Core/Rendering/Renderer.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

PerlinVisualizerScene::PerlinVisualizerScene()
{
    const siv::PerlinNoise::seed_type seed = 1234u; /*<-- Use for testing same seed*/
    perlin.reseed(seed);
}

PerlinVisualizerScene::~PerlinVisualizerScene()
{
}

void PerlinVisualizerScene::Update(float DeltaTime)
{
}

static float frequency = 0.15;
static float Limit = 0.8;

void PerlinVisualizerScene::Render()
{
    Terra::Renderer::RenderScene(m_CameraManager.GetCamera());
    
    for (int i = 0; i < 100; ++i)
    {
        for (int j = 0; j < 100; ++j)
        {
            float noiseVal = perlin.noise2D_01(j * frequency,i * frequency);
            float colorval = noiseVal > Limit ? 1.f : 0.f;
            
            const auto pos = glm::vec3(j,i,0);
            const auto size = glm::vec3(1,1,0);
            const auto color = glm::vec4(colorval,colorval,colorval,1.f);
            Terra::Renderer::DrawQuad(pos,size,color);
        }
    }
    
    Terra::Renderer::Flush();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Window");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::DragFloat("Frequency",&frequency,0.001f,0.f,1.f);
    ImGui::DragFloat("Limit",&Limit,0.001f,0.f,1.f);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
