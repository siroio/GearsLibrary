#include <Easing.h>
#include <Mathf.h>

float Easing::Evaluate(Ease type, float time)
{
    return Evaluate(type, time, 1.0f, 0.0f, 0.0f);
}

float Easing::Evaluate(Ease type, float time, float duration)
{
    return Evaluate(type, time, duration, 0.0f, 0.0f);
}

float Easing::Evaluate(Ease type, float time, float duration, float overshootOrAmplitude, float period)
{
    switch (type)
    {
        case Ease::Linear:
            return Linear(time, duration);
        case Ease::InSine:
            return -Mathf::Sin(time / duration * Mathf::HALF_PI) + 1.0f;
        case Ease::OutSine:
            return Mathf::Sin(time / duration * Mathf::HALF_PI);
        case Ease::InOutSine:
            return -0.5f * (Mathf::Cos(Mathf::PI * time / duration) - 1.0f);
        case Ease::InQuad:
            return (time /= duration) * time;
        case Ease::OutQuad:
            return -(time / duration) * time;
        case Ease::InOutQuad:
            if ((time /= duration * 0.5f) < 1.0f) return 0.5f * time * time;
            return 0.5f * ((--time) * (time - 2.0f) - 1.0f);
        case Ease::InQuart:
            return (time /= duration) * time * time * time;
        case Ease::OutQuart:
            return -((time = time / duration - 1.0f) * time * time * time - 1.0f);
        case Ease::InOutQuart:
            if ((time /= duration * 0.5f) < 1.0f) return 0.5f * time * time * time * time;
            return -0.5f * ((time -= 2.0f) * time * time * time - 2.0f);
        case Ease::InExpo:
            return (time == 0.0f) ? 0.0f : Mathf::Pow(2.0f, 10.0f * (time / duration - 1.0f));
        case Ease::OutExpo:
            if (time == duration) return 1;
            return (-Mathf::Pow(2.0f, -10.0f * time / duration) + 1.0f);
        case Ease::InOutExpo:
            if (time == 0.0f) return 0.0f;
            if (time == duration) return 1.0f;
            if ((time /= duration * 0.5f) < 1.0f) return 0.5f * Mathf::Pow(2.0f, 10.0f * (time - 1));
            return 0.5f * (-Mathf::Pow(2.0f, -10.0f * --time) + 2.0f);
        case Ease::InBack:
            return (time /= duration) * time * ((overshootOrAmplitude + 1) * time - overshootOrAmplitude);
        case Ease::OutBack:
            return ((time = time / duration - 1.0f) * time * ((overshootOrAmplitude + 1.0f) * time + overshootOrAmplitude) + 1.0f);
        case Ease::InOutBack:
            if ((time /= duration * 0.5f) < 1.0f) return 0.5f * (time * time * (((overshootOrAmplitude *= (1.525f)) + 1.0f) * time - overshootOrAmplitude));
            return 0.5f * ((time -= 2.0f) * time * (((overshootOrAmplitude *= (1.525f)) + 1.0f) * time + overshootOrAmplitude) + 2.0f);
        case Ease::InBounce:
            return 0.0f;
        case Ease::OutBounce:
            return 0.0f;
        case Ease::InOutBounce:
            return 0.0f;
        case Ease::InCubic:
            return (time /= duration) * time * time;
        case Ease::OutCubic:
            return (time = time / duration - 1.0f) * time * time + 1;
        case Ease::InOutCubic:
            if ((time /= duration * 0.5f) < 1.0f) return 0.5f * time * time * time;
            return 0.5f * ((time -= 2.0f) * time * time + 2.0f);
        case Ease::InQuint:
            return (time /= duration) * time * time * time * time;
        case Ease::OutQuint:
            return ((time = time / duration - 1.0f) * time * time * time * time + 1.0f);
        case Ease::InOutQuint:
            if ((time /= duration * 0.5f) < 1) return 0.5f * time * time * time * time * time;
            return 0.5f * ((time -= 2.0f) * time * time * time * time + 2.0f);
        case Ease::InCirc:
            return -(Mathf::Sqrt(1.0f - (time /= duration) * time) - 1.0f);
        case Ease::OutCirc:
            return Mathf::Sqrt(1.0f - (time = time / duration - 1.0f) * time);
        case Ease::InOutCirc:
            if ((time /= duration * 0.5f) < 1.0f) return -0.5f * (Mathf::Sqrt(1.0f - time * time) - 1.0f);
            return 0.5f * (Mathf::Sqrt(1.0f - (time -= 2.0f) * time) + 1.0f);
        case Ease::InElastic:
            float s0;
            if (time == 0.0f) return 0.0f;
            if ((time /= duration) == 1.0f) return 1.0f;
            if (period == 0.0f) period = duration * 0.3f;
            if (overshootOrAmplitude < 1.0f)
            {
                overshootOrAmplitude = 1.0f;
                s0 = period / 4.0f;
            }
            else s0 = period / Mathf::TWO_PI * Mathf::FastAsin(1 / overshootOrAmplitude);
            return -(overshootOrAmplitude * Mathf::Pow(2.0f, 10.0f * (time -= 1.0f)) * Mathf::Sin((time * duration - s0) * Mathf::TWO_PI / period));
        case Ease::OutElastic:
            float s1;
            if (time == 0.0f) return 0.0f;
            if ((time /= duration) == 1.0f) return 1.0f;
            if (period == 0.0f) period = duration * 0.3f;
            if (overshootOrAmplitude < 1.0f)
            {
                overshootOrAmplitude = 1.0f;
                s1 = period / 4.0f;
            }
            else s1 = period / Mathf::TWO_PI * Mathf::FastAsin(1.0f / overshootOrAmplitude);
            return (overshootOrAmplitude * Mathf::Pow(2.0f, -10.0f * time) * Mathf::Sin((time * duration - s1) * Mathf::TWO_PI / period) + 1.0f);
        case Ease::InOutElastic:
            float s;
            if (time == 0.0f) return 0.0f;
            if ((time /= duration * 0.5f) == 2.0f) return 1.0f;
            if (period == 0.0f) period = duration * (0.3f * 1.5f);
            if (overshootOrAmplitude < 1.0f)
            {
                overshootOrAmplitude = 1.0f;
                s = period / 4.0f;
            }
            else s = period / Mathf::TWO_PI * Mathf::FastAsin(1.0f / overshootOrAmplitude);
            if (time < 1.0f) return -0.5f * (overshootOrAmplitude * Mathf::Pow(2.0f, 10.0f * (time -= 1.0f)) * Mathf::Sin((time * duration - s) * Mathf::TWO_PI / period));
            return overshootOrAmplitude * Mathf::Pow(2.0f, -10.0f * (time -= 1.0f)) * Mathf::Sin((time * duration - s) * Mathf::TWO_PI / period) * 0.5f + 1;
    }

    // outquad
    return -(time /= duration) * (time - 2);
}

float Easing::CustomEase(float time, float duration, float overshootOrAmplitude, float period, CustomFunc function)
{
    return function(time, duration, overshootOrAmplitude, period);
}

float Easing::Linear(float time, float duration)
{
    return time / duration;
}
