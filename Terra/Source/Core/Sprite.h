#pragma once
#include <mat4x4.hpp>
#include <memory>
#include <string>

namespace Terra
{
    class Texture;
    class Sprite
    {
    public:
        Sprite(const std::string& texturePath);
        virtual ~Sprite();

        virtual void Draw() const;
        virtual void Update(float deltaTime);
        void SetPosition(glm::vec3 position);
        void SetScale(const glm::vec3& scale);
        void SetFlipX(bool flipX);
        void SetFlipY(bool flipY);

        glm::vec3 GetPosition() const;
        glm::vec3 GetScale() const;
        glm::mat4 GetTransform() const;
        bool GetFlipX() const;
        bool GetFlipY() const;
        
    protected:
        std::shared_ptr<Texture> m_Texture;
        glm::mat4 m_Transform;
        bool m_FlipXAxis = false;
        bool m_FlipYAxis = false;
    };
}

