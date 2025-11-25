#include "CameraManager.h"
#include <GLFW/glfw3.h>

#include "Core/Application.h"

void CameraManager::Update(const float deltaTime)
{
    CheckCameraMovement(deltaTime);
}

void CameraManager::CheckCameraMovement(float deltaTime)
{
    const auto window = Terra::Application::GetApplication()->GetWindow()->GetWindow();

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        auto newPos = m_camera.GetPosition();
        newPos.z += -5 * deltaTime;
        m_camera.SetPosition(newPos);
        return;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        auto newPos = m_camera.GetPosition();
        newPos.z += 5 * deltaTime;
        m_camera.SetPosition(newPos);
        return;
    }
    
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        auto newPos = m_camera.GetPosition();
        newPos.y += m_CameraSpeed * deltaTime;
        m_camera.SetPosition(newPos);
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        auto newPos = m_camera.GetPosition();
        newPos.y += -m_CameraSpeed * deltaTime;
        m_camera.SetPosition(newPos);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        auto newPos = m_camera.GetPosition();
        newPos.x += -m_CameraSpeed * deltaTime;
        m_camera.SetPosition(newPos);
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        auto newPos = m_camera.GetPosition();
        newPos.x += m_CameraSpeed * deltaTime;
        m_camera.SetPosition(newPos);
    }
}

Terra::Camera& CameraManager::GetCamera()
{
    return m_camera;
}

glm::vec3 CameraManager::GetPosition() const
{
    return m_camera.GetPosition();
}
