#include "Shader.h"
#include <fstream>
#include <iostream>

namespace Terra
{
    std::string Shader::ReadFile(const std::filesystem::path Path)
    {
        std::ifstream file(Path);

        if (!file.is_open())
        {
            std::cerr << "Failed to open file " << Path << "\n";
        }

        std::ostringstream contentStream;
        contentStream << file.rdbuf();
        return contentStream.str();
    }

    
    GLuint Shader::CreateShader(const std::filesystem::path& VertexPath, const std::filesystem::path& FragmentPath)
    {
        //Get source code from file and convert to characters.
        
        std::string VertexShaderSource = ReadFile(VertexPath);
        std::string FragmentShaderSource = ReadFile(FragmentPath);

        const char* VertexShaderCode = VertexShaderSource.c_str();
        const char* FragmentShaderCode = FragmentShaderSource.c_str();

        //Create vertext shader
        GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(VertexShader, 1, &VertexShaderCode, NULL);
        glCompileShader(VertexShader);

        //Check if shader has compiled
        GLint isCompiled = 0;
        glGetShaderiv(VertexShader,GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            char infoLog[1024];
            glGetShaderInfoLog(VertexShader, 1024, NULL, infoLog);
            std::cerr << "SHADER_VERT_COMPILATION_ERROR:" << "\n" << infoLog << "\n";
            glDeleteShader(VertexShader);
            return -1;
        }

        //Create Fragment shader
        GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(FragmentShader, 1, &FragmentShaderCode, NULL);
        glCompileShader(FragmentShader);

        //Check if shader has compiled
        isCompiled = 0;
        glGetShaderiv(FragmentShader,GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            char infoLog[1024];
            glGetShaderInfoLog(FragmentShader, 1024, NULL, infoLog);
            std::cerr << "SHADER_FRAG_COMPILATION_ERROR:" << "\n" << infoLog << "\n";
            glDeleteShader(FragmentShader);
            return -1;
        }

        //Program linking
        GLuint ProgramID = glCreateProgram();
        glAttachShader(ProgramID, VertexShader);
        glAttachShader(ProgramID, FragmentShader);
        glLinkProgram(ProgramID);

        //Check if linking is successfull
        GLint isLinked = 0;
        glGetProgramiv(ProgramID,GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE)
        {
            char infoLog[1024];
            glGetProgramInfoLog(ProgramID, 1024, NULL, infoLog);
            std::cerr << "PROGRAM_LINKING_ERROR:" << "\n" << infoLog << "\n";
            glDeleteProgram(ProgramID);
            glDeleteShader(FragmentShader);
            glDeleteShader(VertexShader);

            return -1;
        }

        glDeleteShader(VertexShader);
        glDeleteShader(FragmentShader);
        
        return ProgramID;
    }
}
