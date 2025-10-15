#pragma once
namespace Terra
{
    class Scene
    {
        public:

        virtual ~Scene() = default;

        virtual void Update(float DeltaTime) = 0;
        virtual void Render() = 0;
    };
}
