#pragma once
namespace Terra
{
    class Scene
    {
        public:

        virtual ~Scene() = default;

        virtual void Update(float DeltaTime) = 0;
        virtual void Render() = 0;

        virtual void OnScreenResize(float width, float height) {}
        virtual void OnInputPressed(int key, int scancode, int mods) {}
        virtual void OnInputHeld(int key, int scancode, int mods) {}
        virtual void OnInputReleased(int key, int scancode, int mods) {}
        virtual void OnMouseClicked(int button, int mods, float posX, float posY) {}
        virtual void OnMouseReleased(int button, int mods, float posX, float posY) {}
        virtual void OnMouseScroll(double offsetX, double offsetY) {}
    };
}
