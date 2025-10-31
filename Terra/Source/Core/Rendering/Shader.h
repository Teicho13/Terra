#pragma once
#include <filesystem>
#include <fwd.hpp>
#include <glad/glad.h>

namespace Terra
{
    class Shader
    {
    public:
        Shader(const std::string& VertextPath, const std::string& FragmentPath);
        ~Shader();
        void Bind();
        void Unbind();

        void SetInt(const std::string& name, int value);
        void SetMat4(const std::string& name, const glm::mat4& matrix);
        
        static GLuint CreateShader(const std::string& VertextPath, const std::string& FragmentPath);
        static std::string ReadFile(const std::string& Path);

    private:
        unsigned int m_ID;
    };
}
