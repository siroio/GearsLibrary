#ifndef GEAR_Vector2_H
#define GEAR_Vector2_H

#include <string>
#include<iosfwd>

struct Vector3;

/**
 * @brief 2次元ベクトルを表す構造体
 */
struct Vector2
{

public:
    union
    {
        struct
        {
            float x, y;///< xとyの座標
        };
        float xy[2];///< xとyの座標を配列にしたもの
    };

public:
    /**
     * @brief コピーコンストラクタ
     * @param[in] v コピー元のベクトル
     */
    Vector2(const Vector2& v) = default;
    /**
     * @brief コンストラクタ
     * @param[in] x X成分の値
     * @param[in] y Y成分の値
     */
    Vector2(float x, float y);
    /**
     * @brief コンストラクタ
     * @param[in] xy X,Y成分の値が等しい値
     */
    explicit Vector2(float xy);
    /**
     * @brief コンストラクタ
     * @param[in] v 3次元ベクトル
     * @details v.z成分は無視される
     */
    explicit Vector2(const Vector3& v);

public:
    /**
     * @brief ゼロベクトルを返す
     * @return Vector2(0, 0)
     */
    static Vector2 Zero();

    /**
     * @brief 単位ベクトルを返す
     * @return Vector2(1, 1)
     */
    static Vector2 One();

    /**
     * @brief 上方向を表すベクトルを返す
     * @return Vector2(0, 1)
     */
    static Vector2 Up();

    /**
     * @brief 下方向を表すベクトルを返す
     * @return Vector2(0, -1)
     */
    static Vector2 Down();

    /**
     * @brief 左方向を表すベクトルを返す
     * @return Vector2(-1, 0)
     */
    static Vector2 Left();

    /**
     * @brief 右方向を表すベクトルを返す
     * @return Vector2(1, 0)
     */
    static Vector2 Right();

public:
    /**
    * @brief 2つのベクトルのドット積を返します
    * @param v1 ドット積を取りたいベクトル1
    * @param v2 ドット積を取りたいベクトル2
    * @return float ベクトル1とベクトル2のドット積
    */
    static float Dot(const Vector2& v1, const Vector2& v2);

    /**
    * @brief 2つのベクトルのなす角度を返します
    * @param from 角度を計算する始点ベクトル
    * @param to 角度を計算する終点ベクトル
    * @return float ベクトルfromとベクトルtoのなす角度（ラジアン）
    */
    static float Angle(const Vector2& from, const Vector2& to);

    /**
    * @brief 2つのベクトルのなす角度を符号付きで返します
    * @param from 角度を計算する始点ベクトル
    * @param to 角度を計算する終点ベクトル
    * @return float ベクトルfromとベクトルtoのなす角度（ラジアン）
    * 符号は、fromからtoに向かう回転方向に対応する符号です
    */
    static float SignedAngle(const Vector2& from, const Vector2& to);

    /**
    * @brief 2つのベクトル間の距離を返します
    * @param a 距離を計算するベクトル1
    * @param b 距離を計算するベクトル2
    * @return float ベクトルaとベクトルbの間の距離
    */
    static float Distance(const Vector2& a, const Vector2& b);

    /**
    * @brief ベクトルの大きさの2乗を返します
    * @param vector 大きさの2乗を求めたいベクトル
    * @return float ベクトルの大きさの2乗
    */
    static float SqrMagnitude(const Vector2& vector);

    /**
    * @brief ベクトルの大きさを返します
    * @param vector 大きさを求めたいベクトル
    * @return float ベクトルの大きさ
    */
    static float Magnitude(const Vector2& vector);

    /**
    * @brief 2つのベクトルのうち、各要素が小さい方を返します
    * @param lhs 最小値を求めるベクトル1
    * @param rhs 最小値を求めるベクトル2
    * @return Vector2 各要素が小さい方のベクトル
    */
    static Vector2 Min(const Vector2& lhs, const Vector2& rhs);

    /**
    * @brief 2つのベクトルのうち、より大きい値を持つ要素を持つVector2を返す
    * @param lhs 比較するベクトルの一つ
    * @param rhs 比較するベクトルのもう一つ
    * @return Vector2 各要素が大きい方のベクトル
    */
    static Vector2 Max(const Vector2& lhs, const Vector2& rhs);

    /**
    * @brief 2つのベクトルの要素ごとの積を計算し、その結果をVector2として返す
    * @param a 1つ目のベクトル
    * @param b 2つ目のベクトル
    * @return Vector2 要素ごとの積の結果を持つベクトル
    */
    static Vector2 Scale(const Vector2& a, const Vector2& b);

    /**
    * @brief 入射ベクトルに対する法線ベクトルを使用して反射ベクトルを計算する
    * @param inDirection 入射ベクトル
    * @param inNormal 法線ベクトル
    * @return Vector2 反射ベクトル
    */
    static Vector2 Reflect(const Vector2& inDirection, const Vector2& inNormal);

    /**
    * @brief 与えられたベクトルに直交する、垂直なベクトルを計算して返す
    * @param inDirection 入力ベクトル
    * @return Vector2 inDirectionに垂直なベクトル
    */
    static Vector2 Perpendicular(const Vector2& inDirection);

    /**
    * @brief 指定された2つのベクトル間の方向を返す
    * @param from ベクトルの始点
    * @param to ベクトルの終点
    * @return Vector2 ベクトルの方向
    */
    static Vector2 Direction(const Vector2& from, const Vector2& to);
    /**
    * @brief 指定されたベクトルを正規化し、長さを1にする
    * @param v 正規化するベクトル
    * @return Vector2 正規化されたベクトル
    */
    static Vector2 Normalize(const Vector2& v);
    static Vector2 ClampMagnitude(const Vector2& vector, float maxLength);
    static Vector2 Lerp(const Vector2& a, const Vector2& b, float t);
    static Vector2 LerpUnclamped(const Vector2& a, const Vector2& b, float t);
    static Vector2 MoveTowards(const Vector2& current, const Vector2& target, float maxDistanceDelta);

public:
    void Set(const Vector2& v);
    void Set(float x, float y);
    void Set(float xy);
    void Set0();
    void Scale(const Vector2& scale);
    void Normalize();
    float SqrMagnitude() const;
    float Magnitude() const;


public:
    std::string ToString() const;

public:
    void operator = (const Vector2& v);
};

Vector2 operator - (const Vector2& v);
Vector2 operator + (const Vector2& v1, const Vector2& v2);
Vector2 operator - (const Vector2& v1, const Vector2& v2);
Vector2 operator * (const Vector2& v1, const Vector2& v2);
Vector2 operator * (const Vector2& v1, float scale);
Vector2 operator * (float scale, const Vector2& v1);
Vector2 operator / (const Vector2& v1, const Vector2& v2);
Vector2 operator / (const Vector2& v1, float scale);

Vector2& operator += (Vector2& v1, const Vector2& v2);
Vector2& operator -= (Vector2& v1, const Vector2& v2);
Vector2& operator *= (Vector2& v1, const Vector2& v2);
Vector2& operator *= (Vector2& v1, float scale);
Vector2& operator /= (Vector2& v1, const Vector2& v2);
Vector2& operator /= (Vector2& v1, float scale);

bool operator == (const Vector2& v1, const Vector2 v2);
bool operator != (const Vector2& v1, const Vector2 v2);

std::ostream& operator << (std::ostream& stream, const Vector2& v);

#endif // !GEAR_Vector2_H
