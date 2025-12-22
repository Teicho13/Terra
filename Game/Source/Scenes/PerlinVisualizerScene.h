#pragma once
#include <PerlinNoise.hpp>

#include "Core/Scene.h"
#include "../CameraManager.h"

class PerlinVisualizerScene : public Terra::Scene
{
public:
    PerlinVisualizerScene();
    ~PerlinVisualizerScene() override;
    
    void Update(float DeltaTime) override;
    void Render() override;
    
    void OnInputPressed(int key, int scancode, int mods) override {}
    void OnInputReleased(int key, int scancode, int mods) override {}
    void OnInputHeld(int key, int scancode, int mods) override {}

private:
    CameraManager m_CameraManager;
    siv::PerlinNoise perlin;
};
