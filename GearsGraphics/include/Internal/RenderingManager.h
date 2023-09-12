#pragma once
#include <Internal/ISystem.h>
#include <Singleton.h>
#include <Internal/FuncOrderConstant.h>
#include <FuncDefinition.h>
#include <FuncOrderDefinition.h>
#include <concepts>

namespace Glib::Internal
{
    class Renderer;
    class CameraBase;
}

namespace Glib::Internal::Concepts
{
    template<class T>
    concept IsRenderer = std::derived_from<T, Renderer>;
}

namespace Glib::Internal::Graphics
{
    class RenderingManager :
        public Interface::ISystem,
        public SingletonPtr<RenderingManager>,
        public Function::DrawOrderSet<Order::Draw::OBJECT>
    {
        RenderingManager() = default;
        friend WeakPtr<RenderingManager> SingletonPtr<RenderingManager>::Instance();

    public:
        bool Initialize();
        void Update();
        void Draw();

        template<class T> requires Concepts::IsRenderer<T>
        void AddRenderer(const std::shared_ptr<T>& renderer);

    private:

        template<class T> requires HasDrawFunc<T, void, WeakPtr<CameraBase>>
        void AddDraw(const std::shared_ptr<T> renderer);
        void AddDraw(...)
        {}

        template<class T> requires HasDrawShadowFunc<T, void, WeakPtr<CameraBase>>
        void AddDrawShadow(const std::shared_ptr<T> renderer);
        void AddDrawShadow(...)
        {}
    };
}
