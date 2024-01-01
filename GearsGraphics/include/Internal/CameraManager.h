#pragma once
#include <Internal/ISystem.h>
#include <Internal/FuncOrderConstant.h>
#include <FuncOrderDefinition.h>
#include <Singleton.h>
#include <list>

namespace Glib::Internal
{
    class CameraBase;
}

namespace Glib::Internal::Graphics
{
    /**
     * @brief カメラ管理クラス
     */
    class CameraManager :
        public Interface::ISystem,
        public SingletonPtr<CameraManager>,
        public Function::DrawOrderSet<Order::Draw::CAMERA>
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

