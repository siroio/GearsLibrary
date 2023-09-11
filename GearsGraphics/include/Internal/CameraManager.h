#pragma once
#include <Internal/ISystem.h>
#include <Singleton.h>
#include <list>

namespace Glib::Internal
{
    class CameraBase;
}

namespace Glib::Internal::Graphics
{
    class CameraManager :
        public Interface::ISystem,
        public SingletonPtr<CameraManager>
    {
        CameraManager() = default;
        friend WeakPtr<CameraManager> SingletonPtr<CameraManager>::Instance();

    public:
        void BeginDraw();
        void Draw();
        void AddCamera(const WeakPtr<CameraBase>& camera);
        const std::list<WeakPtr<CameraBase>> Cameras() const;

    private:
        std::list<WeakPtr<CameraBase>> cameras_;
    };
}
