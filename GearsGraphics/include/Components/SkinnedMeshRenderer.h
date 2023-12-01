#pragma once
#include <Internal/DX12/ConstantBuffer.h>
#include <Internal/Renderer.h>
#include <Internal/CameraBase.h>
#include <Components/Transform.h>
#include <Matrix4x4.h>
#include <Bone.h>
#include <array>
#include <vector>

namespace Glib
{
    class SkinnedMeshRenderer :
        public Internal::Renderer
    {
    public:
        SkinnedMeshRenderer();
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
        std::array<Matrix4x4, 128> boneMatrix_;
        std::vector<Bone> bones_;
        std::vector<WeakPtr<Transform>> boneTransforms_;
        Internal::Graphics::ConstantBuffer worldConstantBuffer_;
        Internal::Graphics::ConstantBuffer boneConstantBuffer_;
        unsigned int meshID_{ UINT_MAX };
        bool isEnabled_{ false };
    };
}
