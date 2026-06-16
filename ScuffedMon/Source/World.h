#pragma once
#include <memory>
#include "Core/Scene.h"
#include "Core/Rendering/Camera.h"

class Player;
class World : public Terra::Scene
{
public:
    World();
    ~World() override;

    void Update(float DeltaTime) override;
    void Render() override;

private:
    Terra::Camera m_camera;
    std::unique_ptr<Player> m_Player = nullptr;
};
