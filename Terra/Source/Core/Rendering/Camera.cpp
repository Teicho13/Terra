#include "Camera.h"
#include <gtc/matrix_transform.hpp>

#include "Core/Application.h"

namespace Terra
{
    Camera::Camera()
        :m_ProjectionMatrix(glm::ortho(0.f, Application::GetApplication()->GetWindowBuffer().x, 0.f, Application::GetApplication()->GetWindowBuffer().y, -1.0f, 1.0f)) , m_ViewMatrix(1.0f)
    {
        m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void Camera::UpdateViewMatrix()
    {
        glm::mat4 transform = glm::scale(glm::mat4(1.f),glm::vec3(m_ZoomLevel,m_ZoomLevel,1.0f)) * glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
        m_ViewMatrix = glm::inverse(transform);
        m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }
}
