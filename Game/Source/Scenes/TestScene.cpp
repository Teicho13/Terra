#include "TestScene.h"
#include "Core/Rendering/Shader.h"
#include <stb_image.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Core/Application.h"

//Vertices coordinates triangle
GLfloat vertices[] =
{
    -0.5f, -0.5f, 0.f,      0.f, 0.f,
    -0.5f,  0.5f, 0.f,      0.f, 1.f,
     0.5f,  0.5f, 0.f,      1.f, 1.f,
     0.5f, -0.5f, 0.0f,     1.f, 0.f
};

//Order in which the vertices will be drawed (optimization)
GLuint Indices[] =
{
    0, 2, 1, 
    0, 3, 2
};

//Temporary variables

glm::mat4 trans = glm::mat4(1.0f);
float scale = 1.0f;

TestScene::TestScene()
{
    //Create shader using default vertex and fragment (currently hardcoded location)
    std::filesystem::path VertexPath("E:/GameDev/Personal/Other/Terra/Terra/Resources/Shaders/DefaultVertex.vert");
    std::filesystem::path FragmentPath("E:/GameDev/Personal/Other/Terra/Terra/Resources/Shaders/DefaultFragment.frag");
    m_TestShader = Terra::Shader::CreateShader(VertexPath,FragmentPath);

    //Create Objects
    glGenVertexArrays(1, &m_TestVAO);
    glGenBuffers(1, &m_TestVBO);
    glGenBuffers(1,&m_TestEBO);
        
    glBindVertexArray(m_TestVAO);

    //Bind vertex buffer and link it to the vertices.
    glBindBuffer(GL_ARRAY_BUFFER, m_TestVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Bind elements buffer and link it to the indeces.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_TestEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

    //Link VBO attributes (Coordinates & Texture coordinate)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    //Unbind all objects (order matters)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Gets ID of uniform called "scale"
    m_UniformID = glGetUniformLocation(m_TestShader, "scale");
    
    //Load test image
    int ImgWidth, ImgHeight, numColCh;
    unsigned char* bytes = stbi_load("E:/GameDev/Personal/Other/Terra/Terra/Resources/Textures/boomkin.jpg", &ImgWidth, &ImgHeight, &numColCh, 0);

    //Create Texture object and bind ID
    glGenTextures(1,&m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    //Set texture parameter
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    //assign texture to opengl object
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,ImgWidth,ImgHeight,0,GL_RGB,GL_UNSIGNED_BYTE,bytes);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    //unbind object and free memory
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(bytes);

    //Get shader texture uniform variable location (Tex 0 - 16).
    GLint textureUniform = glGetUniformLocation(m_TestShader, "tex0");
    glUseProgram(m_TestShader);
    glUniform1i(textureUniform, 0);
    
    m_TransformUniformID = glGetUniformLocation(m_TestShader, "trans");
}

TestScene::~TestScene()
{
    //Cleanup all objects

    glDeleteTextures(1,&m_TextureID);
    glDeleteVertexArrays(1, &m_TestVAO);
    glDeleteBuffers(1, &m_TestVBO);
    glDeleteBuffers(1, &m_TestEBO);
    glDeleteProgram(m_TestShader);
}

void TestScene::Update(float DeltaTime)
{
    //Rotate object and scale it for testing.
    
    trans = glm::rotate(trans, DeltaTime * 0.3f,glm::vec3(0.0f, 0.0f, 1.0f));
    constexpr float amp = (1.0f - 0.1f) / 2.0f;
    scale = (0.1f + amp) + amp * std::sin(Terra::Application::GetTime());
}

void TestScene::Render()
{
    //Make sure we have the correct window size.
    const auto WindowBuffer = Terra::Application::get()->GetWindowBuffer();
    glViewport(0, 0, static_cast<GLsizei>(WindowBuffer.x), static_cast<GLsizei>(WindowBuffer.y));

    //Activate our shader.
    glUseProgram(m_TestShader);

    //Set vertex uniform values
    glUniform1f(m_UniformID, scale);
    glUniformMatrix4fv(m_TransformUniformID, 1, GL_FALSE, glm::value_ptr(trans));

    //Bind Texture and VAO objects in order for OpenGL to use it. 
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glBindVertexArray(m_TestVAO);

    //Draw all vertices.
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
