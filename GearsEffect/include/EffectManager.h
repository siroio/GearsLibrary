#pragma once
#include <string_view>

namespace Glib
{
    class EffectManager
    {
    public:
        static bool Load(unsigned int id, std::string_view path);
    };
}
