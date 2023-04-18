#include "Mathf.h"
#include <cmath>

float Mathf::Inverse(float v)
{
    return 1.0f / v;
}

float Mathf::Sin(float f)
{
    return sinf(f);
}

float Mathf::Cos(float f)
{
    return cosf(f);
}

float Mathf::Tan(float f)
{
    return tanf(f);
}

float Mathf::Asin(float f)
{
    return asinf(f);
}

float Mathf::Acos(float f)
{
    return acosf(f);
}

float Mathf::Atan(float f)
{
    return atanf(f);
}

float Mathf::Atan2(float y, float x)
{
    return atan2f(y, x);
}

float Mathf::Abs(float f)
{
    return f > 0 ? f : -f;
}

int Mathf::Abs(int f)
{
    return f > 0 ? f : -f;
}

float Mathf::Min(float a, float b)
{
    return a < b ? a : b;
}

int Mathf::Min(int a, int b)
{
    return a < b ? a : b;
}

float Mathf::Max(float a, float b)
{
    return a > b ? a : b;
}

int Mathf::Max(int a, int b)
{
    return a > b ? a : b;
}

float Mathf::Pow(const float a, const float b)
{
    return expf(logf(a) * b);
}

float Mathf::Sqrt(const float a)
{
    return Pow(a, 0.5f);
}

float Mathf::Exp(float power)
{
    return expf(power);
}

float Mathf::Log(float f, float p)
{
    return logf(f) / logf(p);
}

float Mathf::Log(float f)
{
    return logf(f);
}

float Mathf::Log10(float f)
{
    return log10f(f);
}

float Mathf::Ceil(float f)
{
    return ceilf(f);
}

float Mathf::Floor(float f)
{
    return floorf(f);
}

float Mathf::Round(float f)
{
    return roundf(f);
}

int Mathf::CeilToInt(float f)
{
    return static_cast<int>(ceilf(f));
}

int Mathf::FloorToInt(float f)
{
    return static_cast<int>(floorf(f));
}

int Mathf::RoundToInt(float f)
{
    return static_cast<int>(roundf(f));
}

float Mathf::Sign(float f)
{
    return f >= 0.0f ? 1.0f : -1.0f;
}

bool Mathf::Approximately(float a, float b)
{
    return Abs(b - a) < Max(0.000001f * Max(Abs(a), Abs(b)), Epsilon * 8);
}

float Mathf::Clamp(float value, float min, float max)
{
    if (value < min)
    {
        value = min;
    }
    else if (value > max)
    {
        value = max;
    }

    return value;
}

int Mathf::Clamp(int value, int min, int max)
{
    if (value < min)
    {
        value = min;
    }
    else if (value > max)
    {
        value = max;
    }

    return value;
}

float Mathf::Clamp01(float value)
{
    if (value < 0.0f)
    {
        return 0.0f;
    }
    else if (value < 1.0f)
    {
        return 1.0f;
    }
    else
    {
        return value;
    }
}

float Mathf::Lerp(float a, float b, float t)
{
    return a + (b - a) * Clamp01(t);
}

float Mathf::LerpUnclamped(float a, float b, float t)
{
    return a + (b - a) * t;
}

float Mathf::LerpAngle(float a, float b, float t)
{
    float delta = Repeat((b - a), 360.0f);
    if (delta > 180.0f)
    {
        delta -= 360.0f;
    }

    return a + delta * Clamp01(t);
}

float Mathf::InverseLerp(float a, float b, float value)
{
    if (a != b)
    {
        return Clamp01((value - a) / (b - a));
    }
    else
    {
        return 0.0f;
    }
}

float Mathf::Repeat(float t, float length)
{
    return Clamp(t - Floor(t / length) * length, 0.0f, length);
}

float Mathf::DeltaAngle(float current, float target)
{
    float delta = Repeat((target - current), 360.0f);
    if (delta > 180.0f)
    {
        delta -= 360.0f;
    }

    return delta;
}

float Mathf::MoveTowards(float current, float target, float maxDelta)
{
    if (Abs(target - current) <= maxDelta)
    {
        return target;
    }

    return target + Sign(target - current) * maxDelta;
}

float Mathf::MoveTowardsAngle(float current, float target, float maxDelta)
{
    float deltaAngle = DeltaAngle(current, target);
    if (-maxDelta < deltaAngle && deltaAngle < maxDelta)
    {
        return target;
    }
    target = current + deltaAngle;

    return MoveTowards(current, target, maxDelta);
}

float Mathf::SmoothStep(float from, float to, float t)
{
    t = Clamp01(t);
    t = -2.0f * t * t * t + 3.0f * t * t;

    return to * t + from * (1.0f - t);
}

float Mathf::Gamma(float value, float absmax, float gamma)
{
    bool negative = value < 0.0f;
    float absVal = Abs(value);
    if (absVal > absmax)
    {
        return negative ? -absVal : absVal;
    }
    float result = Pow(absVal / absmax, gamma) * absmax;

    return negative ? -result : result;
}
