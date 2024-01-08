﻿#pragma once
#include <Internal/ISystem.h>
#include <Singleton.h>
#include <Internal/FuncOrderConstant.h>
#include <FuncOrderDefinition.h>

namespace Glib::Internal::Effect
{
    class EffekserManager :
        public Interface::ISystem,
        public SingletonPtr<EffekserManager>,
        public Function::DrawOrderSet<Order::Draw::EFFECT>
    {
        EffekserManager() = default;
        friend WeakPtr<EffekserManager> SingletonPtr<EffekserManager>::Instance();
    };

}
