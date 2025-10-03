#pragma once
#include <memory>
#include "Rendering/Window.h"
namespace Terra 
{
    class Application
    {
    public:
        Application();
        ~Application();

        void Run();

    private:
        bool m_IsRunning = false;
        std::shared_ptr<Window> m_Window;
    };
}


