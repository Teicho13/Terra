#include "ResourceManager.h"
#include "Rendering/Texture.h"

namespace Terra
{
    ResourceManager& ResourceManager::GetInstance()
    {
        static ResourceManager s_ResourceManager;
        return s_ResourceManager;
    }

    std::shared_ptr<Texture> ResourceManager::GetTexture(const std::string& path)
    {
        //Look for texture
        const auto search = m_Textures.find(path);
        if (search != m_Textures.end())
        {
            //Return value
            return search->second;
        }

        //Create and insert new texture
        m_Textures.insert(make_pair(path, std::make_shared<Texture>(path)));
        return m_Textures[path]; 
        
    }
}
