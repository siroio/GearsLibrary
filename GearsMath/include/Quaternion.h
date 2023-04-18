#ifndef QUATERNION_H
#define QUATERNION_H

#include <iosfwd>

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
            float q[4];
        };
    };

public:
    Quaternion(const Quaternion& q) = default;
    Quaternion(float x, float y, float z, float w);

public:
    /**
    @brief 回転なしのクォータニオンを返す
    @return 回転なしのクォータニオン
    */
    static Quaternion Identity();

    /**
    @brief 2つの回転間の角度を返す
    @param q1
    @param q2
    @return q1からq2の角度
    */
    static float Angle(const Quaternion& q1, const Quaternion& q2);

    /**
    @brief axis の周りを deg 度回転するクォータニオンを作成する
    @param deg 回転角度
    @param axis 回転軸
    @return 回転後のクォータニオン
    */
    static Quaternion AngleAxis(float deg, const Vector3& axis);

    static float Dot(const Quaternion& q1, const Quaternion& q2);
    static Quaternion Normalize(const Quaternion& q);
    static Quaternion Euler(float x, float y, float z);
    static Quaternion Euler(const Vector3& euler);
    static Quaternion Inverse(const Quaternion& q);
    static Quaternion Serp(const Quaternion& a, const Quaternion& b, float t);
    static Quaternion SerpUnclamped(const Quaternion& a, const Quaternion& b, float t);

public:
    Quaternion Normalized() const;
};

Quaternion operator - (const Quaternion& v);
Quaternion operator + (const Quaternion& v1, const Quaternion& v2);
Quaternion operator - (const Quaternion& v1, const Quaternion& v2);
Quaternion operator * (const Quaternion& v1, const Quaternion& v2);
Quaternion operator * (const Quaternion& v1, float scale);
Quaternion operator * (float scale, const Quaternion& v1);

Quaternion& operator *= (Quaternion& v1, const Quaternion& v2);
Quaternion& operator *= (Quaternion& v1, float scale);

std::ostream& operator << (std::ostream& stream, const Quaternion& v);

#endif // !QUATERNION_H
