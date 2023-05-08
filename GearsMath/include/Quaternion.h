#ifndef QUATERNION_H
#define QUATERNION_H

#include <iosfwd>
#include <string>
#include <array>

struct Vector3;

struct Quaternion
{
    union
    {
        struct
        {
            float x, y, z, w;
        };
        struct
        {
            std::array<float, 4> q;
        };
    };

public:
    Quaternion(const Quaternion& q) = default;
    explicit Quaternion();
    explicit Quaternion(float x, float y, float z, float w);

public:
    /**
     * @brief ��]�Ȃ��̃N�H�[�^�j�I����Ԃ�
     * @return ��]�Ȃ��̃N�H�[�^�j�I��
    */
    static Quaternion Identity();

    /**
     * @brief 2�̉�]�Ԃ̊p�x��Ԃ�
     * @param q1
     * @param q2
     * @return q1����q2�̊p�x
    */
    static float Angle(const Quaternion& q1, const Quaternion& q2);

    /**
     * @brief axis �̎���� angle �x��]����N�H�[�^�j�I�����쐬����
     * @param angle ��]�p�x
     * @param axis ��]��
     * @return ��]��̃N�H�[�^�j�I��
    */
    static Quaternion AngleAxis(float angle, const Vector3& axis);

    static float Dot(const Quaternion& q1, const Quaternion& q2);
    static Quaternion Normalize(const Quaternion& q);
    static Quaternion Euler(float x, float y, float z);
    static Quaternion Euler(const Vector3& euler);
    static Quaternion Inverse(const Quaternion& q);
    static Quaternion Serp(const Quaternion& a, const Quaternion& b, float t);
    static Quaternion SerpUnclamped(const Quaternion& a, const Quaternion& b, float t);
    static Quaternion FromToRotation(const Vector3& fromDirection, const Vector3& toDirection);
    static Quaternion LookRotation(const Vector3& view, const Vector3& up);
    static Quaternion LookRotation(const Vector3& view);
    static Quaternion RotateTowards(const Quaternion& from, const Quaternion& to, float maxDegreesDelta);

public:
    void Set(const Quaternion& q);
    void Set(float x, float y, float z, float w);
    void Set(float xyzw);
    void SetIdentity();
    void SetLookRotation(const Vector3& view, const Vector3& up);
    void EulerAngles(const Vector3& angles);
    void Normalize();
    Vector3 EulerAngles() const;
    Quaternion Normalized() const;
    std::string ToString() const;

private:
    static Vector3 InternalMakePositive(Vector3& euler);

public:
    void operator = (const Quaternion& v);
    float operator [] (const size_t index) const;
    float& operator [] (const size_t index);
};

Quaternion operator - (const Quaternion& q);
Quaternion operator + (const Quaternion& q1, const Quaternion& q2);
Quaternion operator - (const Quaternion& q1, const Quaternion& q2);
Quaternion operator * (const Quaternion& q1, const Quaternion& q2);
Quaternion operator * (const Quaternion& q1, float scalar);
Quaternion operator * (float scalar, const Quaternion& q1);

Quaternion& operator *= (Quaternion& q1, const Quaternion& q2);
Quaternion& operator *= (Quaternion& q1, float scalar);

bool operator == (const Quaternion& q1, const Quaternion& q2);
bool operator != (const Quaternion& q1, const Quaternion& q2);

std::ostream& operator << (std::ostream& stream, const Quaternion& q);

#endif // !QUATERNION_H
