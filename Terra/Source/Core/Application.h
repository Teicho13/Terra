#pragma once
#include <memory>
#include <vector>

#include "Scene.h"
#include "Rendering/Window.h"
namespace Terra 
{
    class Application
    {
    public:
        Application();
        ~Application();

        void Run();
        
        glm::vec2 GetWindowBuffer() const;
        static Application* Get();
        static float GetTime();

        template <typename TScene>
        requires(std::is_base_of_v<Scene, TScene>)
        void PushScene() { m_Scenes.push_back(std::make_unique<TScene>()); }
    
    private:
        bool m_IsRunning = false;
        
        std::shared_ptr<Window> m_Window;
        std::vector<std::unique_ptr<Scene>> m_Scenes;
    };

    inline Application* s_Application = nullptr;
}


