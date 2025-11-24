#pragma once
#include "AnimatedSprite.h"
#include "../Map/Map.h"
#include "Core/Scene.h"
#include "Core/Rendering/Camera.h"


class TestScene : public Terra::Scene
{
public:
    TestScene();
    ~TestScene() override;
    
    void Update(float DeltaTime) override;
    void Render() override;
    
    void OnInputPressed(int key, int scancode, int mods) override;
    void OnInputReleased(int key, int scancode, int mods) override;
    void OnInputHeld(int key, int scancode, int mods) override;

private:
    Terra::Camera m_camera;
    Terra::AnimatedSprite m_AnimatedSpriteTest;
    Map m_TestMap;
};
