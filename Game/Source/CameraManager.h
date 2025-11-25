#pragma once
#include "Core/Rendering/Camera.h"

class CameraManager
{
public:
    CameraManager() = default;
        
    void Update(float deltaTime);
    void CheckCameraMovement(float deltaTime);
    
    Terra::Camera& GetCamera();
    glm::vec3 GetPosition() const;
    
private:
    float m_CameraSpeed = 200.0f;
    Terra::Camera m_camera;
};
