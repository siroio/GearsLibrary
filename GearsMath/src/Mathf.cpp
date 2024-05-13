#include <Mathf.h>
#include <cmath>

float Mathf::Inverse(const float v)
{
    return 1.0f / v;
}

float Mathf::DegToRad(float degree)
{
    return degree * DEG2RAD;
}

float Mathf::RadToDeg(float rad)
{
    return rad * RAD2DEG;
}

float Mathf::Sin(float f)
{
    return std::sinf(f);
}

float Mathf::Cos(float f)
{
    return std::cosf(f);
}

float Mathf::Tan(float f)
{
    return std::tanf(f);
}

float Mathf::Asin(float f)
{
    return std::asinf(f);
}

float Mathf::FastAsin(float f)
{
    constexpr float FASTASIN_HALF_PI{ 1.5707963050f };
    const bool nonnegative = (f >= 0.0f);
    const float x = Mathf::Abs(f);
    float omx = 1.0f - x;
    if (omx < 0.0f) omx = 0.0f;

    const float root = Mathf::Sqrt(omx);
    // 7-degree minimax approximation
    float result = ((((((-0.0012624911f * x + 0.0066700901f) * x - 0.0170881256f) * x + 0.0308918810f) * x - 0.0501743046f) * x + 0.0889789874f) * x - 0.2145988016f) * x + FASTASIN_HALF_PI;
    result *= root;  // acos(|x|)
    // acos(x) = pi - acos(-x) when x < 0, asin(x) = pi/2 - acos(x)
    return nonnegative ? FASTASIN_HALF_PI - result : result - FASTASIN_HALF_PI;
}

float Mathf::Acos(float f)
{
    return std::acosf(f);
}

float Mathf::Atan(float f)
{
    return std::atanf(f);
}

float Mathf::Atan2(float y, float x)
{
    return std::atan2f(y, x);
}

float Mathf::Abs(float f)
{
    return f >= 0 ? f : -f;
}

int Mathf::Abs(int f)
{
    return f >= 0 ? f : -f;
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
    return std::powf(a, b);
}

float Mathf::Sqrt(const float a)
{
    return std::sqrtf(a);
}

float Mathf::InvSqrt(const float a)
{
    return 1 / Sqrt(a);
}

float Mathf::FastInvSqrt(const float a)
{
    long X, Y;
    float y;
    X = *(long*)&a;
    Y = 0x5F3759DF - (X >> 1); // Magic number!
    y = *(float*)&Y;

    // Newton's method
    constexpr float threehalfs = 1.5F;
    float x2 = a * 0.5F;
    y = y * (threehalfs - (x2 * y * y));
    return y;
}

float Mathf::Exp(float power)
{
    return std::expf(power);
}

float Mathf::Log(float f, float p)
{
    return std::logf(f) / std::logf(p);
}

float Mathf::Log(float f)
{
    return std::logf(f);
}

float Mathf::Log10(float f)
{
    return std::log10f(f);
}

float Mathf::Ceil(float f)
{
    return std::ceilf(f);
}

float Mathf::Floor(float f)
{
    return std::floorf(f);
}

float Mathf::Round(float f)
{
    return std::roundf(f);
}

float Mathf::Truncate(float a)
{
    return std::trunc(a);
}

float Mathf::Mod(float a, float b)
{
    return std::fmod(a, b);
}

int Mathf::CeilToInt(float f)
{
    return static_cast<int>(std::ceilf(f));
}

int Mathf::FloorToInt(float f)
{
    return static_cast<int>(std::floorf(f));
}

int Mathf::RoundToInt(float f)
{
    return static_cast<int>(std::roundf(f));
}

float Mathf::Sign(float f)
{
    return f >= 0.0f ? 1.0f : -1.0f;
}

bool Mathf::Approximately(float a, float b)
{
    return Abs(b - a) < EPSILON;
}

bool Mathf::Tolerance(float a, float t)
{
    return a > 1.0f - t;
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
    else if (value > 1.0f)
    {
        return 1.0f;
    }

    return value;
}

float Mathf::Remap(float value, float fromLow, float fromHigh, float toLow, float toHigh)
{
    return toLow + (toHigh - toLow) * (value - fromLow) / (fromHigh - fromLow);
}

float Mathf::Remap01(float value, float low, float high)
{
    return (value - low) / (high - low);
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

float Mathf::GammaToLinearSpace(float value)
{
    return Pow(2.2f, value);
}

float Mathf::LinearToGammaSpace(float value)
{
    static constexpr float gamma = 1.0f / 2.2f;
    return Pow(value, gamma);
}

float Mathf::TGamma(float x)
{
    return std::tgammaf(x);
}

float Mathf::Hypot(float x, float y)
{
    return std::hypotf(x, y);
}

float Mathf::Erf(float x)
{
    return std::erff(x);
}

float Mathf::Erfc(float x)
{
    return std::erfcf(x);
}
