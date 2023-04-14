
struct Vector3;
struct Quaternion;

struct Matrix4x4
{
public:
    union
    {
        struct
        {
            float m11, m12, m13, m14,
                m21, m22, m23, m24,
                m31, m32, m33, m34,
                m41, m42, m43, m44;
        };
        struct
        {
            float matrix[16];
        };
    };

public:
    Matrix4x4(
        float m11, float m12, float m13, float m14,
        float m21, float m22, float m23, float m24,
        float m31, float m32, float m33, float m34,
        float m41, float m42, float m43, float m44);

public:

    static Matrix4x4 Zero();
    static Matrix4x4 Identity();
};
