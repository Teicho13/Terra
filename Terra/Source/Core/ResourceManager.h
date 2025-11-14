#pragma once
#include <memory>
#include <string>
#include <unordered_map>

namespace Terra
{
    class Texture;
    class ResourceManager
    {
    public:
        ResourceManager(const ResourceManager&) = delete;
        void operator =(const ResourceManager&) = delete;
        
        static ResourceManager& GetInstance();

        std::shared_ptr<Texture> GetTexture(const std::string & path);

    private:
        ResourceManager() = default;
        
        std::unordered_map<std::string, std::shared_ptr<Texture>> m_Textures;
    }; 
}

