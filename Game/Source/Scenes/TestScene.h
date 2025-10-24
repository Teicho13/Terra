#pragma once
#include "Core/Scene.h"
#include "Core/Rendering/Camera.h"
#include "Core/Rendering/Texture.h"
#include "Core/Rendering/VertexArray.h"

class TestScene : public Terra::Scene
{
public:
    TestScene();
    virtual ~TestScene();
    
    void Update(float DeltaTime) override;
    void Render() override;


    void UpdateCameraPos();

    void OnInputPressed(int key, int scancode, int mods) override;
    void OnInputReleased(int key, int scancode, int mods) override;
    void OnInputHeld(int key, int scancode, int mods) override;

private:
    
    Terra::Camera m_camera;
    Terra::VertexArray m_VAO;
    Terra::Texture m_TestTexture;
    
    unsigned int m_TestShader;
    int m_UniformID;
    
    int m_MatrixUniformID;
    
};
