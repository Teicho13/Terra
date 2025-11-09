#pragma once
#include "Core/Scene.h"
#include "Core/Rendering/Camera.h"
#include "Core/Rendering/Texture.h"

class TestScene : public Terra::Scene
{
public:
    TestScene();
    virtual ~TestScene();
    
    void Update(float DeltaTime) override;
    void Render() override;
    
    void OnInputPressed(int key, int scancode, int mods) override;
    void OnInputReleased(int key, int scancode, int mods) override;
    void OnInputHeld(int key, int scancode, int mods) override;

private:
    Terra::Camera m_camera;
    std::shared_ptr<Terra::Texture> m_TestTexture;
    std::shared_ptr<Terra::Texture> m_TestTexture2;
};
