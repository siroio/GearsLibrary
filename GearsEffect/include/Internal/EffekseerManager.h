#pragma once
#include <Internal/ISystem.h>
#include <Singleton.h>
#include <Internal/FuncOrderConstant.h>
#include <FuncOrderDefinition.h>
#include <string_view>

namespace Effekseer
{
    struct Matrix44;
}

struct Matrix4x4;

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
        void Finalize();

        /**
         * @brief エフェクトのロード
         * @param path ファイルパス
         */
        bool Load(unsigned int id, std::string_view path);

    private:
        Effekseer::Matrix44 ToMatrix44(const Matrix4x4& matrix);
    };

}
