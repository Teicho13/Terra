#pragma once
#include "Core/Rendering/Camera.h"

class CameraManager
{
public:
    CameraManager() = default;
        
    void Update(float deltaTime);
    void CheckCameraMovement(float deltaTime);
    void SetCameraLimits(float left, float right, float bottom, float top);
    
    Terra::Camera& GetCamera();
    glm::vec3 GetPosition() const;
    
private:
    float m_CameraSpeed = 400.0f;
    Terra::Camera m_camera;
    
    glm::vec2 m_HorizontalLimit = {0.f,1280.f};
    glm::vec2 m_VerticalLimit = {0.f,1280.f};
};
