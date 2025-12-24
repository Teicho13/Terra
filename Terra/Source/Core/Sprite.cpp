#include "Sprite.h"

#include <ext/matrix_transform.hpp>

#include "Application.h"
#include "ResourceManager.h"
#include "Rendering/Renderer.h"
#include "Rendering/Texture.h"

namespace Terra
{
    Sprite::Sprite(const std::string& texturePath)
        : m_Texture(ResourceManager::GetInstance().GetTexture(texturePath)), m_Transform(glm::mat4(1.f))
    {
        //Set default size based off texture size.
        const auto texture = m_Texture.get();
        SetScale({texture->GetWidth(),texture->GetHeight(),0.f});
    }

    Sprite::~Sprite()
    {
        m_Texture.reset();
    }

    void Sprite::Draw() const
    {
        Renderer::DrawQuad(m_Transform,m_Texture);
    }

    void Sprite::Update(float deltaTime)
    {
    }

    void Sprite::SetPosition(const glm::vec3 position)
    {
        m_Transform[3] = glm::vec4(position, 1.f);
    }

    void Sprite::SetScale(const glm::vec3 scale)
    {
        m_Transform[0] = glm::vec4(glm::normalize(glm::vec3(m_Transform[0])) * scale.x, 0.f);
        m_Transform[1] = glm::vec4(glm::normalize(glm::vec3(m_Transform[1])) * scale.y, 0.f);
        m_Transform[2] = glm::vec4(glm::normalize(glm::vec3(m_Transform[2])) * scale.z, 0.f);
    }

    glm::vec3 Sprite::GetPosition() const
    {
        return m_Transform[3];
    }

    glm::vec3 Sprite::GetScale() const
    {
        return { glm::length(glm::vec3(m_Transform[0])), glm::length(glm::vec3(m_Transform[1])),glm::length(glm::vec3(m_Transform[2])) };
    }

    glm::mat4 Sprite::GetTransform() const
    {
        return m_Transform;
    }
}
