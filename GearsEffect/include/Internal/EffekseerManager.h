#pragma once
#include <Internal/ISystem.h>
#include <Singleton.h>
#include <Internal/FuncOrderConstant.h>
#include <FuncOrderDefinition.h>
#include <string_view>

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
        void Draw();
        void Finalize();

        void Load(std::string_view path);
    private:

    };

}
