#pragma once
#include "Core/Scene.h"

class World : public Terra::Scene
{
public:
    World();
    ~World() override;

    void Update(float DeltaTime) override;
    void Render() override;
};
