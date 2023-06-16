#ifndef GEARS_MATRIX3X3_H
#define GEARS_MATRIX3X3_H

#include <array>

class Matrix3x3
{
    union
    {
        struct
        {
            float m11, m12, m13,
                m21, m22, m23,
                m31, m32, m33,
                m41, m42, m43;
        };
        struct
        {
            std::array<std::array<float, 4>, 4> matrix;
        };
    };


};

#endif // !GEARS_MATRIX3X3_H
