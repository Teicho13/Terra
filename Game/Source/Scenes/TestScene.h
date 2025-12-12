#pragma once
#include "AnimatedSprite.h"
#include "../CameraManager.h"
#include "../Map/ChunkManager.h"
#include "../Map/Map.h"
#include "../Terrain/TerrainGenerator.h"
#include "Core/Scene.h"


class TestScene : public Terra::Scene
{
public:
    TestScene();
    ~TestScene() override;
    
    void Update(float DeltaTime) override;
    void Render() override;
    
    void OnInputPressed(int key, int scancode, int mods) override;
    void OnInputReleased(int key, int scancode, int mods) override;
    void OnInputHeld(int key, int scancode, int mods) override {}

private:
    CameraManager m_CameraManager;
    Terra::AnimatedSprite m_AnimatedSpriteTest;
    Map m_TestMap;
    ChunkManager m_ChunkManager;
    TerrainGenerator m_TerrainGenerator;
};
