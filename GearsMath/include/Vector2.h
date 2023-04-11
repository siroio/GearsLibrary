#ifndef GEAR_Vector2_H
#define GEAR_Vector2_H

#include <string>
#include<iosfwd>

struct Vector3;

/**
 * @brief 2�����x�N�g����\���\����
 */
struct Vector2
{

public:
    union
    {
        struct
        {
            float x, y;///< x��y�̍��W
        };
        float xy[2];///< x��y�̍��W��z��ɂ�������
    };

public:
    /**
     * @brief �R�s�[�R���X�g���N�^
     * @param[in] v �R�s�[���̃x�N�g��
     */
    Vector2(const Vector2& v) = default;
    /**
     * @brief �R���X�g���N�^
     * @param[in] x X�����̒l
     * @param[in] y Y�����̒l
     */
    Vector2(float x, float y);
    /**
     * @brief �R���X�g���N�^
     * @param[in] xy X,Y�����̒l���������l
     */
    explicit Vector2(float xy);
    /**
     * @brief �R���X�g���N�^
     * @param[in] v 3�����x�N�g��
     * @details v.z�����͖��������
     */
    explicit Vector2(const Vector3& v);

public:
    /**
     * @brief �[���x�N�g����Ԃ�
     * @return Vector2(0, 0)
     */
    static Vector2 Zero();

    /**
     * @brief �P�ʃx�N�g����Ԃ�
     * @return Vector2(1, 1)
     */
    static Vector2 One();

    /**
     * @brief �������\���x�N�g����Ԃ�
     * @return Vector2(0, 1)
     */
    static Vector2 Up();

    /**
     * @brief ��������\���x�N�g����Ԃ�
     * @return Vector2(0, -1)
     */
    static Vector2 Down();

    /**
     * @brief ��������\���x�N�g����Ԃ�
     * @return Vector2(-1, 0)
     */
    static Vector2 Left();

    /**
     * @brief �E������\���x�N�g����Ԃ�
     * @return Vector2(1, 0)
     */
    static Vector2 Right();

public:
    /**
    * @brief 2�̃x�N�g���̃h�b�g�ς�Ԃ��܂�
    * @param v1 �h�b�g�ς���肽���x�N�g��1
    * @param v2 �h�b�g�ς���肽���x�N�g��2
    * @return float �x�N�g��1�ƃx�N�g��2�̃h�b�g��
    */
    static float Dot(const Vector2& v1, const Vector2& v2);

    /**
    * @brief 2�̃x�N�g���̂Ȃ��p�x��Ԃ��܂�
    * @param from �p�x���v�Z����n�_�x�N�g��
    * @param to �p�x���v�Z����I�_�x�N�g��
    * @return float �x�N�g��from�ƃx�N�g��to�̂Ȃ��p�x�i���W�A���j
    */
    static float Angle(const Vector2& from, const Vector2& to);

    /**
    * @brief 2�̃x�N�g���̂Ȃ��p�x�𕄍��t���ŕԂ��܂�
    * @param from �p�x���v�Z����n�_�x�N�g��
    * @param to �p�x���v�Z����I�_�x�N�g��
    * @return float �x�N�g��from�ƃx�N�g��to�̂Ȃ��p�x�i���W�A���j
    * �����́Afrom����to�Ɍ�������]�����ɑΉ����镄���ł�
    */
    static float SignedAngle(const Vector2& from, const Vector2& to);

    /**
    * @brief 2�̃x�N�g���Ԃ̋�����Ԃ��܂�
    * @param a �������v�Z����x�N�g��1
    * @param b �������v�Z����x�N�g��2
    * @return float �x�N�g��a�ƃx�N�g��b�̊Ԃ̋���
    */
    static float Distance(const Vector2& a, const Vector2& b);

    /**
    * @brief �x�N�g���̑傫����2���Ԃ��܂�
    * @param vector �傫����2������߂����x�N�g��
    * @return float �x�N�g���̑傫����2��
    */
    static float SqrMagnitude(const Vector2& vector);

    /**
    * @brief �x�N�g���̑傫����Ԃ��܂�
    * @param vector �傫�������߂����x�N�g��
    * @return float �x�N�g���̑傫��
    */
    static float Magnitude(const Vector2& vector);

    /**
    * @brief 2�̃x�N�g���̂����A�e�v�f������������Ԃ��܂�
    * @param lhs �ŏ��l�����߂�x�N�g��1
    * @param rhs �ŏ��l�����߂�x�N�g��2
    * @return Vector2 �e�v�f�����������̃x�N�g��
    */
    static Vector2 Min(const Vector2& lhs, const Vector2& rhs);

    /**
    * @brief 2�̃x�N�g���̂����A���傫���l�����v�f������Vector2��Ԃ�
    * @param lhs ��r����x�N�g���̈��
    * @param rhs ��r����x�N�g���̂������
    * @return Vector2 �e�v�f���傫�����̃x�N�g��
    */
    static Vector2 Max(const Vector2& lhs, const Vector2& rhs);

    /**
    * @brief 2�̃x�N�g���̗v�f���Ƃ̐ς��v�Z���A���̌��ʂ�Vector2�Ƃ��ĕԂ�
    * @param a 1�ڂ̃x�N�g��
    * @param b 2�ڂ̃x�N�g��
    * @return Vector2 �v�f���Ƃ̐ς̌��ʂ����x�N�g��
    */
    static Vector2 Scale(const Vector2& a, const Vector2& b);

    /**
    * @brief ���˃x�N�g���ɑ΂���@���x�N�g�����g�p���Ĕ��˃x�N�g�����v�Z����
    * @param inDirection ���˃x�N�g��
    * @param inNormal �@���x�N�g��
    * @return Vector2 ���˃x�N�g��
    */
    static Vector2 Reflect(const Vector2& inDirection, const Vector2& inNormal);

    /**
    * @brief �^����ꂽ�x�N�g���ɒ�������A�����ȃx�N�g�����v�Z���ĕԂ�
    * @param inDirection ���̓x�N�g��
    * @return Vector2 inDirection�ɐ����ȃx�N�g��
    */
    static Vector2 Perpendicular(const Vector2& inDirection);

    /**
    * @brief �w�肳�ꂽ2�̃x�N�g���Ԃ̕�����Ԃ�
    * @param from �x�N�g���̎n�_
    * @param to �x�N�g���̏I�_
    * @return Vector2 �x�N�g���̕���
    */
    static Vector2 Direction(const Vector2& from, const Vector2& to);
    /**
    * @brief �w�肳�ꂽ�x�N�g���𐳋K�����A������1�ɂ���
    * @param v ���K������x�N�g��
    * @return Vector2 ���K�����ꂽ�x�N�g��
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
