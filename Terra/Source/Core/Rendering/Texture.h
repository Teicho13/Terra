#pragma once
#include <filesystem>

namespace Terra
{
    class Texture
    {
    public:
        Texture() = default;
        Texture(const std::filesystem::path& ImagePath);
        ~Texture();
        void CreateTexture(const std::filesystem::path& ImagePath);
        void Bind() const;
        void Unbind() const;
    private:
        unsigned int m_ID = 0;
    };
}

