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

#include "../WorldObjects/Characters/Player.h"


TestScene::TestScene()
    : m_AnimatedSpriteTest(Terra::FileIO::GetEngineFile("Textures\\ExampleSpriteSheet.png"),3)
{
    m_AnimatedSpriteTest.GetAnimation().SetLooped(true);
    m_AnimatedSpriteTest.GetAnimation().SetFrameSpeed(5.f);
    m_AnimatedSpriteTest.GetAnimation().Play();
    m_AnimatedSpriteTest.SetPosition({300.f,200.f,1.f});

    m_TestMap.SetCamera(&m_CameraManager.GetCamera());
    m_TestMap.SetMapData(Terra::FileIO::GetGameFile("Tilemaps\\Data\\FirstMap.csv"));
    m_TestMap.SetMapTexture(Terra::FileIO::GetGameFile("Tilemaps\\GeneralTiles.png"));

    m_TerrainGenerator.SetTerrainTexture(Terra::FileIO::GetGameFile("Terrain\\BasicTiles.png"));
    m_TerrainGenerator.SetActiveCamera(&m_CameraManager.GetCamera());
    m_TerrainGenerator.StartGeneration();

    constexpr auto tileSize = TerrainGenerator::TILE_SIZE;
    const int ChunkPerScreen = Terra::Application::GetApplication()->GetWindowBuffer().x / (TerrainGenerator::CHUNK_WIDTH * tileSize);
    m_CameraManager.SetCameraLimits(0,((m_TerrainGenerator.GetChunks().size() - ChunkPerScreen) * TerrainGenerator::CHUNK_WIDTH) * tileSize,0,TerrainGenerator::CHUNK_HEIGHT * tileSize);
    m_CameraManager.GetCamera().SetPosition({0,(TerrainGenerator::CHUNK_HEIGHT / 2) * tileSize,0});

    m_Player = std::make_unique<Player>(Terra::FileIO::GetGameFile("Characters\\Player.png"));
    m_Player->GetSprite().SetPosition(m_TerrainGenerator.GetPlayerStartingPosition());
    m_Player->SetTerrainGeneratorRef(&m_TerrainGenerator);
}

TestScene::~TestScene()
{
    //Cleanup all objects
}

void TestScene::Update(float DeltaTime)
{
    m_CameraManager.Update(DeltaTime);
    m_Player->Update(DeltaTime);
    
    m_AnimatedSpriteTest.Update(DeltaTime);
    m_AnimatedSpriteTest.SetScale(glm::vec3(32.f,32.f,1.f));
}

void TestScene::Render()
{
    Terra::Renderer::RenderScene(m_CameraManager.GetCamera());
    
    //m_TestMap.DrawMap();
    //m_AnimatedSpriteTest.Draw();
    m_TerrainGenerator.Render();
    m_Player->Draw();

    //Draw all gathered indices.
    Terra::Renderer::Flush();
    
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Window");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("Camera Position: X: %.2f Y: %.2f Z: %.2f", m_CameraManager.GetPosition().x,m_CameraManager.GetPosition().y,m_CameraManager.GetPosition().z);
    ImGui::Text("Camera ZoomLevel: %.2f", m_CameraManager.GetCamera().GetZoomLevel());
    ImGui::Text("Quads: %d", Terra::RenderStats::s_DrawnQuads);
    ImGui::Text("Textures used: %d", Terra::RenderStats::s_DrawnTextures);
    ImGui::Text("Draw Calls: %d", Terra::RenderStats::s_DrawCalls);
    ImGui::Text("Player Position: %.2f %.2f", m_Player->GetSprite().GetPosition().x,m_Player->GetSprite().GetPosition().y);
    ImGui::Text("Player velocity: %.2f %.2f", m_Player->GetVelocity().x,m_Player->GetVelocity().y);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void TestScene::OnInputPressed(int key, int scancode, int mods)
{
    
    if (key == GLFW_KEY_EQUAL)
    {
        float zoom = m_CameraManager.GetCamera().GetZoomLevel();
        m_CameraManager.GetCamera().SetZoomLevel(zoom - 0.1f);
    }

    if (key == GLFW_KEY_MINUS)
    {
        float zoom = m_CameraManager.GetCamera().GetZoomLevel();
        m_CameraManager.GetCamera().SetZoomLevel(zoom + 0.1f);
    }

    if (key == GLFW_KEY_SPACE)
    {
        if (m_Player)
        {
            m_Player->Jump();
        }
    }
}

void TestScene::OnInputReleased(int key, int scancode, int mods)
{
    if (key == GLFW_KEY_ESCAPE)
    {
        Terra::Application::GetApplication()->RequestEnd();
    }
}
