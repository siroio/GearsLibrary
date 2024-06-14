#pragma once
#include <Internal/DX12/ConstantBufferAddress.h>
#include <Internal/Renderer.h>
#include <Internal/CameraBase.h>
#include <Components/Transform.h>

namespace Glib
{
    class MeshRenderer : public Internal::Renderer
    {
    public:
        MeshRenderer();
        void Start();
        void LateUpdate();
        void Draw(const WeakPtr<Internal::CameraBase>& camera);
        void DrawShadow(const WeakPtr<Internal::CameraBase>& camera);

        /**
         * @brief メッシュのIDを取得
         */
        unsigned int MeshID() const;

        /**
         * @brief メッシュのIDを設定
         */
        void MeshID(unsigned int id);

    private:
        void OnGUI() override;

    private:
        WeakPtr<Transform> transform_;
        Internal::Graphics::ConstantBufferAddress constantBuffer_;
        unsigned int meshID_{ UINT_MAX };
        bool isEnabled_{ false };
    };
}
