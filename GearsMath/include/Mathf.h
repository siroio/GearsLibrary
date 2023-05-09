#ifndef GEARS_MATHF_H
#define GEARS_MATHF_H

#include <limits>

struct Mathf
{
public:
    static constexpr float EPSILON = 1e-5f;
    static constexpr float EPSILON_SRQT = 1e-5f * 1e-5f;
    static constexpr float EPSILON_NORMAL_SQRT = 1e-15f;
    static constexpr float PI = 3.141592653589793238462643383279f;
    static constexpr float DEG2RAD = PI / 180.0f;
    static constexpr float RAD2DEG = 180.0f / PI;
    static constexpr float POSITIVE_INFINITY = std::numeric_limits<float>::infinity();
    static constexpr float NEGATIVE_INFINITY = -std::numeric_limits<float>::infinity();

    static float Inverse(float v);

    static float Sin(float f);
    static float Cos(float f);
    static float Tan(float f);

    static float Asin(float f);
    static float Acos(float f);
    static float Atan(float f);
    static float Atan2(float y, float x);

    static float Abs(float f);
    static int Abs(int f);
    static float Min(float a, float b);
    static int Min(int a, int b);
    static float Max(float a, float b);
    static int Max(int a, int b);

    static float Pow(const float a, const float b);
    static float Sqrt(const float a);
    static float Exp(float power);
    static float Log(float f, float p);
    static float Log(float f);
    static float Log10(float f);
    static float Ceil(float f);
    static float Floor(float f);
    static float Round(float f);
    static int CeilToInt(float f);
    static int FloorToInt(float f);
    static int RoundToInt(float f);

    static float Sign(float f);
    static bool Approximately(float a, float b);
    static bool Tolerance(float a, float t = EPSILON);
    static float Clamp(float value, float min, float max);
    static int Clamp(int value, int min, int max);
    static float Clamp01(float value);
    static float Lerp(float a, float b, float t);
    static float LerpUnclamped(float a, float b, float t);
    static float LerpAngle(float a, float b, float t);
    static float InverseLerp(float a, float b, float value);
    static float Repeat(float t, float length);
    static float DeltaAngle(float current, float target);
    static float MoveTowards(float current, float target, float maxDelta);
    static float MoveTowardsAngle(float current, float target, float maxDelta);
    static float SmoothStep(float from, float to, float t);
    static float Gamma(float value, float absmax, float gamma);
    static float TGamma(float x);
    static float Hypot(float x, float y);
    static float Erf(float x);
    static float Erfc(float x);
};

#endif // !GEARS_MATHF_H
