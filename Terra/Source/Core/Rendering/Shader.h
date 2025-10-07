#pragma once
#include <filesystem>
#include <glad/glad.h>

namespace Terra
{
    class Shader
    {
    public:
        static GLuint CreateShader(const std::filesystem::path& VertextPath, const std::filesystem::path& FragmentPath);

        static std::string ReadFile(const std::filesystem::path Path);
    };
}
