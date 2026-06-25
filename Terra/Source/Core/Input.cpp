#include "Input.h"
#include <GLFW/glfw3.h>

namespace Terra
{
    void Input::BeginInputFrame()
    {
        s_PreviousKeys = s_CurrentKeys;
        s_PreviousMouseButtons = s_CurrentMouseButtons;
        s_PreviousMods = s_CurrentMods;
        s_MouseScrollDelta = { 0.0f, 0.0f };
    }

    bool Input::IsKeyDown(const InputKey key)
    {
        return s_CurrentKeys.contains(key) && s_CurrentKeys[key];
    }

    void Input::OnKey(const int key, const int action, int mods)
    {
        auto inputKey = GLFWToInputKey(key);
        s_CurrentMods = mods;

        if (!inputKey)
            return;
        
        if (action == GLFW_PRESS || action == GLFW_REPEAT)
        {
            s_CurrentKeys[*inputKey] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            s_CurrentKeys[*inputKey] = false;
        }
    }

    void Input::OnMouseScroll(float x, float y)
    {
        s_MouseScrollDelta += glm::vec2{ x, y };
    }

    void Input::OnMouseClick(const int button, const int action, const float posX, const float posY)
    {
        if (action == GLFW_PRESS)
        {
            s_CurrentMouseButtons[button] = true;
            s_LastMousePosition = { posX, posY };
        }
        else if (action == GLFW_RELEASE)
        {
            s_CurrentMouseButtons[button] = false;
            s_LastMousePosition = { posX, posY };
        }
    }

    glm::vec2 Input::GetMouseScrollDelta()
    {
        return s_MouseScrollDelta;
    }

    bool Input::WasKeyPressed(const InputKey key)
    {
        const bool current = s_CurrentKeys.contains(key) && s_CurrentKeys[key];
        const bool previous = s_PreviousKeys.contains(key) && s_PreviousKeys[key];

        return current && !previous;
    }

    bool Input::WasKeyReleased(const InputKey key)
    {
        const bool current = s_CurrentKeys.contains(key) && s_CurrentKeys[key];
        const bool previous = s_PreviousKeys.contains(key) && s_PreviousKeys[key];

        return !current && previous;
    }

    bool Input::IsModDown(const InputMod mod)
    {
        return (s_CurrentMods & static_cast<int>(mod)) != 0;
    }

    bool Input::WasModPressed(const InputMod mod)
    {
        return (s_CurrentMods & static_cast<int>(mod)) != 0 &&
               (s_PreviousMods & static_cast<int>(mod)) == 0;
    }

    bool Input::WasModReleased(const InputMod mod)
    {
        return (s_CurrentMods & static_cast<int>(mod)) == 0 &&
               (s_PreviousMods & static_cast<int>(mod)) != 0;
    }
    
    std::optional<InputKey> Input::GLFWToInputKey(int glfwKey)
    {
        switch (glfwKey)
        {
        case GLFW_KEY_A:      return InputKey::A;
        case GLFW_KEY_D:      return InputKey::D;
        case GLFW_KEY_S:      return InputKey::S;
        case GLFW_KEY_W:      return InputKey::W;
        default:              return std::nullopt;
        }
    }
}
