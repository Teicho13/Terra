#pragma once
#include <optional>
#include <unordered_map>
#include <vec2.hpp>

namespace Terra
{
    enum class InputKey : int
    {
        A = 65,
        D = 68,
        S = 83,
        W = 87
    };

    enum class InputMod : uint8_t
    {
        shift = 0x0001,
        ctrl = 0x0002,
        alt = 0x0004,
    };
    
    class Input
    {
    public:
        static void BeginInputFrame();
        
        static void OnKey(int key, int action, int mods);
        static void OnMouseScroll(float x, float y);
        static void OnMouseClick(int button, int action, float posX, float posY);
        
        static bool IsKeyDown(InputKey key);
        static bool WasKeyPressed(InputKey key);
        static bool WasKeyReleased(InputKey key);

        static bool IsModDown(InputMod mod);
        static bool WasModPressed(InputMod mod);
        static bool WasModReleased(InputMod mod);
        static glm::vec2 GetMouseScrollDelta();

    private:

        static std::optional<InputKey> GLFWToInputKey(int glfwKey);
        
        inline static std::unordered_map<InputKey, bool> s_CurrentKeys;
        inline static std::unordered_map<InputKey, bool> s_PreviousKeys;
        inline static std::unordered_map<int, bool> s_PreviousMouseButtons;
        inline static std::unordered_map<int, bool> s_CurrentMouseButtons;
        
        inline static glm::vec2 s_MouseScrollDelta;
        inline static glm::vec2 s_LastMousePosition;

        inline static int s_CurrentMods = 0;
        inline static int s_PreviousMods = 0;
    };
}

