#pragma once
#include <functional>

/**
 * @brief イージング種類
 */
enum class Ease
{
    Linear,

    InSine,
    OutSine,
    InOutSine,

    InQuad,
    OutQuad,
    InOutQuad,

    InQuart,
    OutQuart,
    InOutQuart,

    InExpo,
    OutExpo,
    InOutExpo,

    InBack,
    OutBack,
    InOutBack,

    InBounce,
    OutBounce,
    InOutBounce,

    InCubic,
    OutCubic,
    InOutCubic,

    InQuint,
    OutQuint,
    InOutQuint,

    InCirc,
    OutCirc,
    InOutCirc,

    InElastic,
    OutElastic,
    InOutElastic,

    Flash,
    InFlash,
    OutFlash,
    InOutFlash,

    Custom,
};

class Easing
{
public:
    using CustomFunc = std::function<float(float, float, float, float)>;

    /**
     * @brief 選択したイージングを計算
     * @param type 種類
     * @param time 時間
     * @return
     */
    static float Evaluate(Ease type, float time);

    /**
     * @brief 選択したイージングを計算
     * @param type 種類
     * @param time
     * @param duration
     * @return
     */
    static float Evaluate(Ease type, float time, float duration);

    /**
     * @brief 選択したイージングを計算
     * @param type 種類
     * @param time
     * @param duration
     * @param overshootOrAmplitude
     * @param period
     * @return
     */
    static float Evaluate(Ease type, float time, float duration, float overshootOrAmplitude, float period);

    /**
     * @brief カスタムしたイージングを計算
     * @param time
     * @param duration
     * @param overshootOrAmplitude
     * @param period
     * @return
     */
    static float CustomEase(float time, float duration, float overshootOrAmplitude, float period, CustomFunc function);

private:
    static float Linear(float time, float duration);
};
