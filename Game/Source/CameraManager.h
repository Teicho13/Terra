#pragma once
#include "Core/Rendering/Camera.h"

class CameraManager
{
public:
    CameraManager(float width, float height);

    void Update(float deltaTime);
    void Resize(float width, float height);
    void SetCameraMovementLimits(float left, float right, float bottom, float top);
    void SetCameraMovementIsLimited(bool newValue);
    void SetCameraPosition(float x, float y, float z);
    
    Terra::Camera& GetCamera();
    
    float GetZoomLevel() const;
    void SetZoomLevel(float value);
    void OnScroll(float deltaY);
    
private:
    void HandleInput(float deltaTime);
    float m_Zoom = 1.0f;
    float m_CameraSpeed = 400.0f;

    Terra::Camera m_Camera;
    glm::vec2 m_CameraSize = {1080.0f, 720.0f};
    glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
    
    bool m_LimitCameraMovement = false;
    glm::vec2 m_HorizontalLimit = {0.f,1280.f};
    glm::vec2 m_VerticalLimit = {0.f,1280.f};
};
