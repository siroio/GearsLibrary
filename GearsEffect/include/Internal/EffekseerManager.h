#pragma once
#include <Internal/ISystem.h>
#include <Singleton.h>
#include <Internal/FuncOrderConstant.h>
#include <FuncOrderDefinition.h>
#include <string_view>

namespace Effekseer
{
    struct Matrix44;
    struct Matrix43;
    typedef int Handle;
}

struct Matrix4x4;
struct Vector3;
struct Color;

using EffectHandle = Effekseer::Handle;
constexpr EffectHandle EFFEKSEER_INVALID_HANDLE = -1;

namespace Glib::Internal::Effect
{
    class EffekseerManager :
        public Interface::ISystem,
        public SingletonPtr<EffekseerManager>,
        public Function::DrawOrderSet<Order::Draw::EFFECT>
    {
        EffekseerManager() = default;
        friend WeakPtr<EffekseerManager> SingletonPtr<EffekseerManager>::Instance();

    public:
        bool Initialize();
        void Update();
        void Draw();

        /**
         * @brief エフェクトのロード
         * @param path ファイルパス
         */
        bool Load(unsigned int id, std::string_view path);

        /**
         * @brief エフェクトが追加されているか
         * @param id エフェクトID
         */
        bool Contains(unsigned int id);

        /**
         * @brief エフェクトが存在しているか
         * @param handle
         * @return
         */
        bool Exists(EffectHandle handle);

        /**
         * @brief エフェクトを再生
         * @param id エフェクトID
         * @param position 発生場所
         * @return 再生したエフェクトのハンドル
         */
        EffectHandle Play(unsigned int id, const Vector3& position);

        /**
         * @brief エフェクトを一時停止
         * @param handle
         */
        void Pause(EffectHandle handle);

        /**
         * @brief エフェクトを再開
         * @param handle
         */
        void UnPause(EffectHandle handle);

        /**
         * @brief エフェクトを停止
         * @param handle
         */
        void Stop(EffectHandle handle);

        /**
         * @brief 再生スピードを設定
         * @param handle
         */
        void SetSpeed(EffectHandle handle, float speed);

        /**
         * @brief 色を指定
         * @param handle
         * @param color
         */
        void SetColor(EffectHandle handle, const Color& color);

        /**
         * @brief 再生中のエフェクトのターゲット位置を設定
         * @param handle
         * @param position
         */
        void SetTargetPosition(EffectHandle handle, const Vector3& position);

        /**
         * @brief 変換行列を設定
         * @param handle
         * @param matrix
         */
        void SetMatrix(EffectHandle handle, const Matrix4x4& matrix);

    private:
        Effekseer::Matrix44 ToMatrix44(const Matrix4x4& matrix);
        Effekseer::Matrix43 ToMatrix43(const Matrix4x4& matrix);
    };
}
