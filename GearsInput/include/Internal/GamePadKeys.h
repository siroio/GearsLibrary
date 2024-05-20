#pragma once

namespace Glib
{
    enum class GPADKey : unsigned
    {
        UP = 0x0001,
        DOWN = 0x0002,
        LEFT = 0x0004,
        RIGHT = 0x0008,
        START = 0x0010,
        SELECT = 0x0020,
        LEFT_TRIGGER = 0x0100,
        RIGHT_TRIGGER = 0x0200,
        A = 0x1000,
        CROSS = 0x1000,
        B = 0x2000,
        CIRCLE = 0x2000,
        X = 0x4000,
        SQUARE = 0x4000,
        Y = 0x8000,
        TRIANGLE = 0x8000,
        LEFT_STICK = 0x0040,
        RIGHT_STICK = 0x0080
    };
}
