#pragma once
#include <algorithm>
#include <glm.hpp>

namespace Terra
{
    class Camera
    {
    public:
        Camera(float left, float right, float bottom, float top);
        void SetProjectionMatrix(float left, float right, float bottom, float top);

        void SetPosition(const glm::vec3& position);
        void SetRotation(float rotation);
        void SetZoom(float zoom);
        
        glm::vec3 GetPosition() const;
        float GetRotation() const;
        float GetZoom() const;
        
        glm::mat4 GetViewMatrix() const;
        glm::mat4 GetProjectionMatrix() const;
        glm::mat4 GetProjectionViewMatrix() const;
    private:

        void RecalculateViewMatrix();
        
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ProjectionViewMatrix;

        glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
        float m_Rotation = 0.0f;
        float m_Zoom = 1.0f;
    }; 
}

