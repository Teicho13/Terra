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

        std::shared_ptr<Window> m_Window;
    };
}


