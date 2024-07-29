#pragma once
#include <iosfwd>
#include <string>
#include <array>

struct Vector2;
struct Quaternion;
struct Matrix4x4;

struct Vector3
{

public:
    union
    {
        struct
        {
            float x, y, z;
        };
        struct
        {
            std::array<float, 3> xyz;
        };
    };

public:
    Vector3(const Vector3&) = default;
    explicit Vector3();
    explicit Vector3(float x, float y, float z);
    explicit Vector3(float xyz);
    explicit Vector3(const Vector2& v);

public:
    /**
     * @brief ゼロベクトルを返す
     *
     * @return ゼロベクトル
     */
    static Vector3 Zero();

    /**
     * @brief 一のベクトルを返す
     *
     * @return 一のベクトル
     */
    static Vector3 One();

    /**
     * @brief 前方ベクトルを返す
     *
     * @return 前方ベクトル
     */
    static Vector3 Forward();

    /**
     * @brief 後方ベクトルを返す
     *
     * @return 後方ベクトル
     */
    static Vector3 Back();

    /**
     * @brief 上方向ベクトルを返す
     *
     * @return 上方向ベクトル
     */
    static Vector3 Up();

    /**
     * @brief 下方向ベクトルを返す
     *
     * @return 下方向ベクトル
     */
    static Vector3 Down();

    /**
     * @brief 左方向ベクトルを返す
     *
     * @return 左方向ベクトル
     */
    static Vector3 Left();

    /**
     * @brief 右方向ベクトルを返す
     *
     * @return 右方向ベクトル
     */
    static Vector3 Right();

    /**
     * @brief 2つのベクトルの要素ごとの除算を行う
     *
     * @param v1 被除数のベクトル
     * @param v2 除数のベクトル
     * @param threshold しきい値（デフォルトは1e-5f）
     * @return 除算結果のベクトル
     */
    static Vector3 Divide(const Vector3& v1, const Vector3& v2);

    /**
     * @brief 2つのベクトルのドット積を計算する
     *
     * @param v1 ベクトル1
     * @param v2 ベクトル2
     * @return ベクトル1とベクトル2のドット積
     */
    static float Dot(const Vector3& v1, const Vector3& v2);

    /**
     * @brief 2つのベクトル間の角度を計算する
     *
     * @param from 始点のベクトル
     * @param to 終点のベクトル
     * @return 2つのベクトル間の角度（ラジアン）
     */
    static float Angle(const Vector3& from, const Vector3& to);

    /**
     * @brief 2つのベクトル間の符号付き角度を計算する
     *
     * @param from 始点のベクトル
     * @param to 終点のベクトル
     * @param axis 軸ベクトル
     * @return 2つのベクトル間の符号付き角度（ラジアン）
     */
    static float SignedAngle(const Vector3& from, const Vector3& to, const Vector3& axis);

    /**
     * @brief 2つのベクトル間の距離を計算する
     *
     * @param a ベクトル1
     * @param b ベクトル2
     * @return 2つのベクトル間の距離
     */
    static float Distance(const Vector3& a, const Vector3& b);

    /**
     * @brief ベクトルの二乗ノルムを計算する
     *
     * @param vector 計算するベクトル
     * @return ベクトルの二乗ノルム
     */
    static float SqrMagnitude(const Vector3& vector);

    /**
     * @brief ベクトルのノルムを計算する
     *
     * @param vector 計算するベクトル
     * @return ベクトルのノルム
     */
    static float Magnitude(const Vector3& vector);

    /**
     * @brief 2つのベクトルの要素ごとの最小値を取る
     *
     * @param lhs ベクトル1
     * @param rhs ベクトル2
     * @return 要素ごとの最小値を持つベクトル
     */
    static Vector3 Min(const Vector3& lhs, const Vector3& rhs);

    /**
     * @brief 2つのベクトルの要素ごとの最大値を取る
     *
     * @param lhs ベクトル1
     * @param rhs ベクトル2
     * @return 要素ごとの最大値を持つベクトル
     */
    static Vector3 Max(const Vector3& lhs, const Vector3& rhs);

    /**
     * @brief 2つのベクトルの外積を計算する
     *
     * @param a ベクトル1
     * @param b ベクトル2
     * @return 2つのベクトルの外積を持つベクトル
     */
    static Vector3 Cross(const Vector3& a, const Vector3& b);

    /**
     * @brief 2つのベクトルの要素ごとの積を計算する
     *
     * @param a ベクトル1
     * @param b ベクトル2
     * @return 要素ごとの積を持つベクトル
     */
    static Vector3 Scale(const Vector3& a, const Vector3& b);

    /**
     * @brief ベクトルとスカラーの要素ごとの積を計算する
     *
     * @param v ベクトル
     * @param scalar スカラー値
     * @return ベクトルとスカラーの要素ごとの積を持つベクトル
     */
    static Vector3 Scale(const Vector3& v, float scalar);

    /**
     * @brief 入射ベクトルを法線ベクトルに対して反射させる
     *
     * @param inDirection 入射ベクトル
     * @param inNormal 法線ベクトル
     * @return 反射ベクトル
     */
    static Vector3 Reflect(const Vector3& inDirection, const Vector3& inNormal);

    /**
     * @brief ベクトルを指定の法線ベクトルに投影する
     *
     * @param vector 投影するベクトル
     * @param onNormal 法線ベクトル
     * @return 投影ベクトル
     */
    static Vector3 Project(const Vector3& vector, const Vector3& onNormal);

    /**
     * @brief ベクトルを指定の平面に投影する
     *
     * @param vector 投影するベクトル
     * @param planeNormal 平面の法線ベクトル
     * @return 投影ベクトル
     */
    static Vector3 ProjectOnPlane(const Vector3& vector, const Vector3& planeNormal);

    /**
     * @brief ベクトルの長さを制限する
     *
     * @param vector 制限するベクトル
     * @param maxLength 最大長さ
     * @return 長さが制限されたベクトル
     */
    static Vector3 ClampMagnitude(const Vector3& vector, float maxLength);

    /**
     * @brief ベクトルを正規化する
     *
     * @param v 正規化するベクトル
     * @return 正規化されたベクトル
     */
    static Vector3 Normalize(const Vector3& v);

    /**
     * @brief 2つのベクトルを線形補間する
     *
     * @param a 補間開始のベクトル
     * @param b 補間終了のベクトル
     * @param t 補間係数（0.0f から 1.0f の範囲）
     * @return 線形補間されたベクトル
     */
    static Vector3 Lerp(const Vector3& a, const Vector3& b, float t);

    /**
     * @brief 2つのベクトルを線形補間する（範囲制限なし）
     *
     * @param a 補間開始のベクトル
     * @param b 補間終了のベクトル
     * @param t 補間係数
     * @return 線形補間されたベクトル
     */
    static Vector3 LerpUnclamped(const Vector3& a, const Vector3& b, float t);

    /**
     * @brief 2つのベクトルを球面線形補間する
     *
     * @param a 補間開始のベクトル
     * @param b 補間終了のベクトル
     * @param t 補間係数（0.0f から 1.0f の範囲）
     * @return 球面線形補間されたベクトル
     */
    static Vector3 Slerp(const Vector3& a, const Vector3& b, float t);

    /**
     * @brief 2つのベクトルを球面線形補間する（範囲制限なし）
     *
     * @param a 補間開始のベクトル
     * @param b 補間終了のベクトル
     * @param t 補間係数
     * @return 球面線形補間されたベクトル
     */
    static Vector3 SlerpUnclamped(const Vector3& a, const Vector3& b, float t);

    /**
     * @brief 現在のベクトルからターゲットベクトルまで移動する
     *
     * @param current 現在のベクトル
     * @param target 目標ベクトル
     * @param maxDistanceDelta 最大移動距離
     * @return 移動後のベクトル
     */
    static Vector3 MoveTowards(const Vector3& current, const Vector3& target, float maxDistanceDelta);

public:
    void Set(const Vector3& v);
    void Set(float x, float y, float z);
    void Set(float xyz);
    void Set0();
    float SqrMagnitude() const;
    float Magnitude() const;
    void Normalize();
    Vector3 Normalized() const;
    std::string ToString() const;

public:
    void operator = (const Vector3& v);
    float operator [] (const size_t index) const;
    float& operator [] (const size_t index);
};

Vector3 operator - (const Vector3& v);
Vector3 operator + (const Vector3& v1, const Vector3& v2);
Vector3 operator - (const Vector3& v1, const Vector3& v2);
Vector3 operator * (const Vector3& v1, const Vector3& v2);
Vector3 operator * (const Quaternion& q, const Vector3& v);
Vector3 operator * (const Vector3& v1, float scalar);
Vector3 operator * (float scalar, const Vector3& v1);
Vector3 operator * (const Vector3& v, const Matrix4x4& m);
Vector3 operator * (const Quaternion& q, const Vector3& v);
Vector3 operator / (const Vector3& v1, const Vector3& v2);
Vector3 operator / (const Vector3& v1, float scalar);

Vector3& operator += (Vector3& v1, const Vector3& v2);
Vector3& operator -= (Vector3& v1, const Vector3& v2);
Vector3& operator *= (Vector3& v1, const Vector3& v2);
Vector3& operator *= (Vector3& v, const Matrix4x4& m);
Vector3& operator *= (Vector3& v, float scalar);
Vector3& operator /= (Vector3& v1, const Vector3& v2);
Vector3& operator /= (Vector3& v, float scalar);

bool operator == (const Vector3& v1, const Vector3 v2);
bool operator != (const Vector3& v1, const Vector3 v2);

std::ostream& operator << (std::ostream& stream, const Vector3& v);
