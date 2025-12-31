#pragma once
#include "AnimatedSprite.h"
#include "../CameraManager.h"
#include "../Map/Map.h"
#include "../Terrain/TerrainGenerator.h"
#include "Core/Scene.h"


class Player;
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
    void OnScreenResize(float width, float height) override;
    void OnMouseScroll(double offsetX, double offsetY) override;
    void OnMouseClicked(int button, int mods, float posX, float posY) override;

private:
    CameraManager m_CameraManager;
    Terra::AnimatedSprite m_AnimatedSpriteTest;
    Map m_TestMap;
    TerrainGenerator m_TerrainGenerator;
    std::unique_ptr<Player> m_Player = nullptr;
};
