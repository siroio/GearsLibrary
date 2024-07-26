#pragma once
#include <iosfwd>
#include <string>
#include <array>

struct Vector3;

struct Vector4
{

public:
    union
    {
        struct
        {
            float x, y, z, w;
        };
        struct
        {
            std::array<float, 4> xyzw;
        };
    };

public:
    Vector4(const Vector4&) = default;
    explicit Vector4();
    explicit Vector4(float x, float y, float z, float w);
    explicit Vector4(float xyzw);

public:
    static Vector4 Zero();
    static Vector4 One();

public:
    /**
     * @brief 2つのベクトルのドット積を計算する
     *
     * @param v1 ベクトル1
     * @param v2 ベクトル2
     * @return ベクトル1とベクトル2のドット積
     */
    static float Dot(const Vector4& v1, const Vector4& v2);

    /**
     * @brief 2つのベクトル間の距離を計算する
     *
     * @param a ベクトル1
     * @param b ベクトル2
     * @return 2つのベクトル間の距離
     */
    static float Distance(const Vector4 a, const Vector4 b);

    /**
     * @brief ベクトルの二乗ノルムを計算する
     *
     * @param vector 計算するベクトル
     * @return ベクトルの二乗ノルム
     */
    static float SqrMagnitude(const Vector4& vector);

    /**
     * @brief ベクトルのノルムを計算する
     *
     * @param vector 計算するベクトル
     * @return ベクトルのノルム
     */
    static float Magnitude(const Vector4& vector);

    /**
     * @brief 2つのベクトルの要素ごとの最小値を取る
     *
     * @param lhs ベクトル1
     * @param rhs ベクトル2
     * @return 要素ごとの最小値を持つベクトル
     */
    static Vector4 Min(const Vector4& lhs, const Vector4& rhs);

    /**
     * @brief 2つのベクトルの要素ごとの最大値を取る
     *
     * @param lhs ベクトル1
     * @param rhs ベクトル2
     * @return 要素ごとの最大値を持つベクトル
     */
    static Vector4 Max(const Vector4& lhs, const Vector4& rhs);

    /**
     * @brief 2つのベクトルの要素ごとの積を計算する
     *
     * @param a ベクトル1
     * @param b ベクトル2
     * @return 要素ごとの積を持つベクトル
     */
    static Vector4 Scale(const Vector4& a, const Vector4& b);

    /**
     * @brief ベクトルとスカラーの要素ごとの積を計算する
     *
     * @param v ベクトル
     * @param scalar スカラー値
     * @return ベクトルとスカラーの要素ごとの積を持つベクトル
     */
    static Vector4 Scale(const Vector4& v, float scalar);

    /**
     * @brief 2つのベクトルの線形補間を計算する
     *
     * @param a ベクトル1
     * @param b ベクトル2
     * @param t 補間係数（0から1の範囲）
     * @return 補間結果
     */
    static Vector4 Lerp(const Vector4& a, const Vector4& b, float t);

    /**
     * @brief 2つのベクトルの線形補間を計算する（制限なし）
     *
     * @param a ベクトル1
     * @param b ベクトル2
     * @param t 補間係数
     * @return 補間結果
     */
    static Vector4 LerpUnclamped(const Vector4& a, const Vector4& b, float t);

    /**
     * @brief ベクトルを正規化する
     *
     * @param v 正規化するベクトル
     * @return 正規化されたベクトル
     */
    static Vector4 Normalize(const Vector4& v);

    /**
     * @brief ベクトルの反射ベクトルを計算する
     *
     * @param inDirection 入力ベクトル
     * @param inNormal 面の法線ベクトル
     * @return 反射ベクトル
     */
    static Vector4 Reflect(const Vector4& inDirection, const Vector4& inNormal);

    /**
     * @brief ベクトルを別のベクトルに投影する
     *
     * @param vector 投影するベクトル
     * @param onNormal 投影先のベクトル
     * @return 投影結果のベクトル
     */
    static Vector4 Project(const Vector4& vector, const Vector4& onNormal);


public:
    void Set(const Vector4& v);
    void Set(float x, float y, float z, float w);
    void Set(float xyzw);
    void Set0();
    float SqrMagnitude() const;
    float Magnitude() const;
    std::string ToString() const;

public:
    void operator = (const Vector4& v);
    float operator [] (const size_t index) const;
    float& operator [] (const size_t index);
};

Vector4 operator - (const Vector4& v);
Vector4 operator + (const Vector4& v1, const Vector4& v2);
Vector4 operator - (const Vector4& v1, const Vector4& v2);
Vector4 operator * (const Vector4& v1, const Vector4& v2);
Vector4 operator * (const Vector4& v1, float scalar);
Vector4 operator * (float scalar, const Vector4& v1);
Vector4 operator / (const Vector4& v1, const Vector4& v2);
Vector4 operator / (const Vector4& v1, float scalar);

Vector4& operator += (Vector4& v1, const Vector4& v2);
Vector4& operator -= (Vector4& v1, const Vector4& v2);
Vector4& operator *= (Vector4& v1, const Vector4& v2);
Vector4& operator *= (Vector4& v1, float scalar);
Vector4& operator /= (Vector4& v1, const Vector4& v2);
Vector4& operator /= (Vector4& v1, float scalar);

bool operator == (const Vector4& v1, const Vector4 v2);
bool operator != (const Vector4& v1, const Vector4 v2);

std::ostream& operator<<(std::ostream& stream, const Vector4& v);
