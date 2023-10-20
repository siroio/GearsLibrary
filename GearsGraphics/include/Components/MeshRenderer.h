#pragma once
#include <Internal/DX12/ConstantBuffer.h>
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
        void Draw(const WeakPtr<Internal::CameraBase> camera);
        void DrawShadow(const WeakPtr<Internal::CameraBase> camera);

        /**
         * @brief ���b�V����ID���擾
         */
        unsigned int MeshID() const;

        /**
         * @brief ���b�V����ID��ݒ�
         */
        void MeshID(unsigned int id);

    private:
        WeakPtr<Transform> transform_;
        Internal::Graphics::ConstantBuffer constantBuffer_;
        unsigned int meshID_{ UINT_MAX };
        bool isEnabled_;
    };
}