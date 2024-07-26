#pragma once
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
     * @brief 回転なしのクォータニオンを返す
     * @return 回転なしのクォータニオン
    */
    static Quaternion Identity();

    /**
     * @brief 2つの回転間の角度を返す
     * @param q1
     * @param q2
     * @return q1からq2の角度
    */
    static float Angle(const Quaternion& q1, const Quaternion& q2);

    /**
     * @brief axis の周りを angle 度回転するクォータニオンを作成する
     * @param angle 回転角度
     * @param axis 回転軸
     * @return 回転後のクォータニオン
    */
    static Quaternion AngleAxis(float angle, const Vector3& axis);

    /**
     * @brief クォータニオンのドット積を計算する
     *
     * @param q1 クォータニオン1
     * @param q2 クォータニオン2
     * @return クォータニオン1とクォータニオン2のドット積
     */
    static float Dot(const Quaternion& q1, const Quaternion& q2);

    /**
     * @brief クォータニオンを正規化する
     *
     * @param q 正規化するクォータニオン
     * @return 正規化されたクォータニオン
     */
    static Quaternion Normalize(const Quaternion& q);

    /**
     * @brief オイラー角からクォータニオンを作成する
     *
     * @param x X軸回りの回転角（ラジアン）
     * @param y Y軸回りの回転角（ラジアン）
     * @param z Z軸回りの回転角（ラジアン）
     * @return オイラー角から作成されたクォータニオン
     */
    static Quaternion Euler(float x, float y, float z);

    /**
     * @brief オイラー角からクォータニオンを作成する
     *
     * @param euler オイラー角を持つベクトル
     * @return オイラー角から作成されたクォータニオン
     */
    static Quaternion Euler(const Vector3& euler);

    /**
     * @brief クォータニオンの逆数を計算する
     *
     * @param q 逆数を求めるクォータニオン
     * @return クォータニオンの逆数
     */
    static Quaternion Inverse(const Quaternion& q);

    /**
     * @brief 2つのクォータニオンを球面線形補間する
     *
     * @param a 補間開始のクォータニオン
     * @param b 補間終了のクォータニオン
     * @param t 補間係数（0.0f から 1.0f の範囲）
     * @return 球面線形補間されたクォータニオン
     */
    static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t);

    /**
     * @brief 2つのクォータニオンを球面線形補間する（範囲制限なし）
     *
     * @param a 補間開始のクォータニオン
     * @param b 補間終了のクォータニオン
     * @param t 補間係数
     * @return 球面線形補間されたクォータニオン
     */
    static Quaternion SlerpUnclamped(const Quaternion& a, const Quaternion& b, float t);

    /**
     * @brief 2つの方向ベクトルの間の回転を計算する
     *
     * @param fromDirection 始点の方向ベクトル
     * @param toDirection 終点の方向ベクトル
     * @return 2つの方向ベクトルの間の回転を表すクォータニオン
     */
    static Quaternion FromToRotation(const Vector3& fromDirection, const Vector3& toDirection);

    /**
     * @brief 目標方向と上方向から回転を計算する
     *
     * @param view 目標方向のベクトル
     * @param up 上方向のベクトル
     * @return 目標方向と上方向から作成されたクォータニオン
     */
    static Quaternion LookRotation(const Vector3& view, const Vector3& up);

    /**
     * @brief 目標方向から回転を計算する
     *
     * @param view 目標方向のベクトル
     * @return 目標方向から作成されたクォータニオン
     */
    static Quaternion LookRotation(const Vector3& view);

    /**
     * @brief 2つのクォータニオンの間を回転する
     *
     * @param from 現在のクォータニオン
     * @param to 目標のクォータニオン
     * @param maxDegreesDelta 最大回転角度（度）
     * @return 指定した角度内で回転したクォータニオン
     */
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
    static float NormalizeAxis(float angle);
    static void MakePositive(Vector3& angles);

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
