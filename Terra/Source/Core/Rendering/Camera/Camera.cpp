#include "Camera.h"
#include <gtc/matrix_transform.hpp>

#include "Core/Application.h"

namespace Terra
{
    Camera::Camera(const float left, const float right, const float bottom, const float top)
        : m_ProjectionMatrix(glm::ortho(left,right,bottom,top,-1.0f, 1.0f)), m_ViewMatrix(1.0f)
    {
        m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void Camera::SetProjectionMatrix(const float left, const float right, const float bottom, const float top)
    {
        m_ProjectionMatrix = glm::ortho(left,right,bottom,top, -1.0f, 1.0f);
        m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void Camera::SetPosition(const glm::vec3& position)
    {
        m_Position = position;
        RecalculateViewMatrix();
    }

    void Camera::SetRotation(const float rotation)
    {
        m_Rotation = rotation;
        RecalculateViewMatrix();
    }

    void Camera::SetZoom(float zoom)
    {
        m_Zoom = zoom;
    }

    float Camera::GetZoom() const
    {
        return m_Zoom;
    }

    glm::vec3 Camera::GetPosition() const
    {
        return m_Position;
    }

    float Camera::GetRotation() const
    {
        return m_Rotation;
    }

    glm::mat4 Camera::GetViewMatrix() const
    {
        return m_ViewMatrix;
    }

    glm::mat4 Camera::GetProjectionMatrix() const
    {
        return m_ProjectionMatrix;
    }

    glm::mat4 Camera::GetProjectionViewMatrix() const
    {
        return m_ProjectionViewMatrix;
    }

    void Camera::RecalculateViewMatrix()
    {
        //glm::mat4 transform = glm::scale(glm::mat4(1.f),glm::vec3(m_Zoom,m_Zoom,1.0f)) * glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
        glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
        m_ViewMatrix = glm::inverse(transform);
        m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;  
    }
}
