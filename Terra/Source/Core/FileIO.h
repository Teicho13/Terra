#pragma once
#include <string>

namespace Terra
{
    class FileIO
    {
    public:
        static std::string GetEngineFile(const std::string& path); 
        static std::string GetGameFile(const std::string& path);
        static std::string GetScuffedMonFile(const std::string& path);
    }; 
}

