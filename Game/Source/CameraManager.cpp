#include "CameraManager.h"
#include <GLFW/glfw3.h>

#include <algorithm>

#include "Core/Application.h"

CameraManager::CameraManager(const float width, const float height)
    :m_Camera(0 * m_Zoom,width * m_Zoom,0,height * m_Zoom)
{
}

void CameraManager::Update(float deltaTime)
{
    HandleInput(deltaTime);
    m_Camera.SetPosition(m_CameraPosition);
}

void CameraManager::Resize(const float width, const float height)
{
    m_CameraSize = { width, height };
    m_Camera.SetProjectionMatrix(0 * m_Zoom,width * m_Zoom,0,height * m_Zoom);
}

void CameraManager::SetCameraMovementLimits(const float left, const float right, const float bottom, const float top)
{
    m_HorizontalLimit.x = left;
    m_HorizontalLimit.y = right;

    m_VerticalLimit.x = bottom;
    m_VerticalLimit.y = top;
}

void CameraManager::SetCameraMovementIsLimited(const bool newValue)
{
    m_LimitCameraMovement = newValue;
}

void CameraManager::SetCameraPosition(float x, float y, float z)
{
    auto newPos = glm::vec3(x, y, z);
    if (m_LimitCameraMovement)
    {
        newPos.y = std::max(newPos.y, m_VerticalLimit.x);
        newPos.y = std::min(newPos.y, m_VerticalLimit.y);

        newPos.x = std::max(newPos.x, m_HorizontalLimit.x);
        newPos.x = std::min(newPos.x, m_HorizontalLimit.y);
    }

    m_CameraPosition = newPos;
    m_Camera.SetPosition(newPos);
}

Terra::Camera& CameraManager::GetCamera()
{
    return m_Camera;
}

glm::vec3 CameraManager::GetCameraPosition() const
{
    return m_CameraPosition;
}

glm::vec2 CameraManager::GetCameraSize() const
{
    return m_CameraSize;
}


float CameraManager::GetZoomLevel() const
{
    return m_Zoom;
}

void CameraManager::SetZoomLevel(const float value)
{
    m_Zoom = std::max(value, 0.1f);
    m_Camera.SetZoom(m_Zoom);
    m_Camera.SetProjectionMatrix(0.0f * m_Zoom,m_CameraSize.x * m_Zoom,0.0f,m_CameraSize.y * m_Zoom);
}

void CameraManager::OnScroll(float deltaY)
{
    m_Zoom -= deltaY * 0.1f;
    m_Zoom = std::max(m_Zoom, 0.1f);
    m_Camera.SetZoom(m_Zoom);
    m_Camera.SetProjectionMatrix(0.0f * m_Zoom,m_CameraSize.x * m_Zoom,0.0f,m_CameraSize.y * m_Zoom);
}

void CameraManager::HandleInput(float deltaTime)
{
    const auto window = Terra::Application::GetApplication()->GetWindow()->GetWindow();

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            auto newPos = m_CameraPosition;
            newPos.z += -5 * deltaTime;
            m_CameraPosition = newPos;
            return;
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            auto newPos = m_CameraPosition;
            newPos.z += 5 * deltaTime;
            m_CameraPosition = newPos;
            return;
        }
    }
    
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        auto newPos = m_CameraPosition;
        if (m_LimitCameraMovement)
        {
            if (newPos.y < m_VerticalLimit.y)
            {
                newPos.y += m_CameraSpeed * deltaTime;
            }
            else
            {
                newPos.y = m_VerticalLimit.y;
            } 
        }
        else
        {
            newPos.y += m_CameraSpeed * deltaTime;
        }
        
        m_CameraPosition = newPos;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        auto newPos = m_CameraPosition;
        if (m_LimitCameraMovement)
        {
            if (newPos.y > m_VerticalLimit.x)
            {
                newPos.y += -m_CameraSpeed * deltaTime;
            }
            else
            {
                newPos.y = m_VerticalLimit.x;
            }  
        }
        else
        {
            newPos.y += -m_CameraSpeed * deltaTime;
        }
        
        m_CameraPosition = newPos;
    }
    
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        auto newPos = m_CameraPosition;
        if (m_LimitCameraMovement)
        {
            if (newPos.x > m_HorizontalLimit.x)
            {
                newPos.x += -m_CameraSpeed * deltaTime;
            }
            else
            {
                newPos.x = m_HorizontalLimit.x;
            } 
        }
        else
        {
            newPos.x += -m_CameraSpeed * deltaTime;
        }
        
        m_CameraPosition = newPos;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        auto newPos = m_CameraPosition;
        if (m_LimitCameraMovement)
        {
            if (newPos.x < m_HorizontalLimit.y)
            {
                newPos.x += m_CameraSpeed * deltaTime;
            }
            else
            {
                newPos.x = m_HorizontalLimit.y;
            }  
        }
        else
        {
            newPos.x += m_CameraSpeed * deltaTime;
        }
        
        m_CameraPosition = newPos;
    }
}
