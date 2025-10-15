#pragma once
#include "Core/Scene.h"
class TestScene : public Terra::Scene
{
public:
    TestScene();
    virtual ~TestScene();
    
    void Update(float DeltaTime) override;
    void Render() override;

private:
    unsigned int m_TestShader;
    unsigned int m_TestVBO;
    unsigned int m_TestVAO;
    unsigned int m_TestEBO;
    unsigned int m_TextureID;
    int m_UniformID;

    int m_TransformUniformID;
    
};
