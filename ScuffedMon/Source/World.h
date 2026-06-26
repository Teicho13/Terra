#pragma once
#include <memory>
#include "Core/Scene.h"
#include "Core/Events/Interfaces/InputListener.h"
#include "Core/Rendering/Camera/CameraManager.h"

class Player;
class World : public Terra::Scene, public Terra::InputListener
{
public:
    World();
    ~World() override;

    void Update(float DeltaTime) override;
    void Render() override;

    /*InputListener*/
    /*void OnInputReleased(int key, int scancode, int mods) override;*/
    

private:
    CameraManager m_CameraManager;
    std::unique_ptr<Player> m_Player = nullptr;
};
