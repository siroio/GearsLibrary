#pragma once
#include <Internal/DX12/ConstantBuffer.h>
#include <Internal/Renderer.h>
#include <Internal/SkinnedRenderer.h>
#include <Internal/CameraBase.h>
#include <Components/Transform.h>
#include <Matrix4x4.h>
#include <Bone.h>
#include <array>
#include <vector>

namespace Glib
{
    class SkinnedMeshRenderer :
        public Internal::Renderer,
        public Internal::SkinnedRenderer
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
        /**
         * @brief ボーンの計算
         */
        void ComputeBone();

        /**
         * @brief ボーンのゲームオブジェクトを生成
         */
        void CreateBoneGameObject();

        /**
         * @brief ボーンの取得
         */
        virtual const std::vector<Bone>& Bones() const override;

        /**
         * @brief ボーンのトランスフォームを取得
         */
        virtual const std::vector<WeakPtr<Transform>>& BoneTransforms() const override;

        /**
         * @brief ボーンの行列を取得
         */
        virtual std::array<Matrix4x4, 512>& BoneMatrix() override;

    private:
        WeakPtr<Transform> transform_;
        std::array<Matrix4x4, 512> boneMatrix_;
        std::vector<Bone> bones_;
        std::vector<WeakPtr<Transform>> boneTransforms_;
        Internal::Graphics::ConstantBuffer worldConstantBuffer_;
        Internal::Graphics::ConstantBuffer boneConstantBuffer_;
        unsigned int meshID_{ UINT_MAX };
        bool isEnabled_{ false };
    };
}
