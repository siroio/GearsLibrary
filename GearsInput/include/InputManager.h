#pragma once
#include <Internal/ISystem.h>
#include <Singleton.h>
#include <FuncOrderDefinition.h>
#include <Internal/FuncOrderConstant.h>
namespace Glib
{
    class InputManager :
        public Glib::Internal::Interface::ISystem,
        public Glib::SingletonPtr<InputManager>,
        public Glib::Internal::Function::UpdateOrderSet<Glib::Internal::Order::Update::INPUT>
    {
    private:
        InputManager() = default;
        friend Glib::WeakPtr<InputManager> SingletonPtr<InputManager>::Instance();

    public:
        ~InputManager();

        bool Initialize();
        void Update();
    };
}