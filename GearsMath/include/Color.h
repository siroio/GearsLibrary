#pragma once
#include <iosfwd>
#include <string>
#include <array>
#include <tuple>

struct Vector4;

struct Color
{
public:
    union
    {
        struct
        {
            float  r, g, b, a;
        };
        std::array<float, 4> rgba;
    };

    using HSV = std::tuple<float, float, float>;

public:
    Color(const Color&) = default;
    Color(const Vector4& vector);
    explicit Color();
    explicit Color(float r, float g, float b, float a = 1.0f);
    explicit Color(float rgba);

public:
    static Color Lerp(const Color& a, const Color& b, float t);
    static Color LerpUnclamped(const Color& a, const Color& b, float t);
    static Color Gamma(float r, float g, float b, float a = 1.0f);
    static Color Gamma(const Color& color);
    static Color Linear(float r, float g, float b, float a = 1.0f);
    static Color Linear(const Color& color);
    static float GrayScale(float r, float g, float b, float a = 1.0f);
    static float GrayScale(const Color& color);
    static float MaxColor(float r, float g, float b);
    static float MaxColor(const Color& color);
    static Color HSVToRGB(float h, float s, float v, bool hdr);
    static Color HSVToRGB(const HSV& hsv, bool hdr);
    static HSV RGBToHSV(float r, float g, float b);
    static HSV RGBToHSV(const Color& color);
    static Color Black();
    static Color White();
    static Color Red();
    static Color Green();
    static Color Blue();
    static Color Yellow();
    static Color Cyan();
    static Color Grey();
    static Color Magenta();

private:
    static void RGBToHSVHelper(float offset, float dominantcolor, float colorone, float colortwo, float& H, float& S, float& V);

public:
    Color Gamma() const;
    Color Linear() const;
    float GrayScale() const;
    float MaxColor() const;
    void Set(const Color& v);
    void Set(float r, float g, float b, float a);
    void Set(float rgba);
    void Set0();
    const float* Raw() const;
    std::string ToString() const;

public:
    void operator = (const Color& c);
    float operator [] (const size_t index) const;
    float& operator[] (const size_t index);
};

Color& operator += (Color& c1, const Color& c2);
Color& operator -= (Color& c1, const Color& c2);
Color& operator *= (Color& c1, const Color& c2);
Color& operator *= (Color& color, float scaler);
Color& operator *= (float scaler, Color& color);
Color& operator /= (Color& color, float scaler);

Color operator + (const Color& c1, const Color& c2);
Color operator - (const Color& c1, const Color& c2);
Color operator * (const Color& c1, const Color& c2);
Color operator * (const Color& color, float scaler);
Color operator * (float scaler, const Color& color);
Color operator / (const Color& color, float scaler);

std::ostream& operator << (std::ostream& stream, const Color& color);
