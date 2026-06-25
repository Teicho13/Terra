#pragma once
#include <memory>
#include "Core/Scene.h"
#include "Core/Rendering/Camera/CameraManager.h"

class Player;
class World : public Terra::Scene
{
public:
    World();
    ~World() override;

    void Update(float DeltaTime) override;
    void Render() override;

private:
    CameraManager m_CameraManager;
    std::unique_ptr<Player> m_Player = nullptr;
};
