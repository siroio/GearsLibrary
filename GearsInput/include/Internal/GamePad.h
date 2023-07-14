#pragma once
#include <dinput.h>
#include <Internal/InputState.h>

namespace Glib::Internal::Input
{
    class GamePad
    {
    public:
        enum class PadNumber : unsigned char
        {
            PAD_1 = 0,
            PAD_2 = 1,
            PAD_3 = 2,
            PAD_4 = 3,
        };

        enum class InputType : unsigned short
        {
            Xinput = 0,
            Dinput = 1
        };

    public:
        bool Initialize();
        void Update();

    private:
        GamePad(GamePad&) = delete;
        GamePad& operator = (const GamePad&) = delete;

    private:
    };
}
