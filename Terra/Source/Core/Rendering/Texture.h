#pragma once
#include <filesystem>

namespace Terra
{
    class Texture
    {
    public:
        Texture(const std::filesystem::path& ImagePath);
        ~Texture();
        
        void Bind(unsigned int slot) const;
        void Unbind() const;

        int GetWidth() const { return m_Width; }
        int GetHeight() const { return m_Height; }
    private:
        unsigned int m_ID;
        std::string m_FilePath;
        int m_Width;
        int m_Height;
        int m_Channels;
    };
}

