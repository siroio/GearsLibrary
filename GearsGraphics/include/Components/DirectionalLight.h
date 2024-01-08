#pragma once
#include <Component.h>
#include <Components/Transform.h>
#include <WeakPtr.h>

struct Color;

namespace Glib
{
    class DirectionalLight : public Component
    {
    public:
        void Start();
        void LateUpdate();
        void Ambient(const Color& ambient);
        void Diffuse(const Color& diffuse);
        void Specular(const Color& specular);
        void OnGUI();
    private:
        WeakPtr<Transform> transform_;
    };
}
