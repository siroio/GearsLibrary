#pragma once
#include <iosfwd>
#include <string>
#include <array>

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

public:
    Color(const Color& color) = default;
    Color(const Vector4& vector);
    explicit Color();
    explicit Color(float r, float g, float b, float a = 1.0f);
    explicit Color(float rgba);

public:
    static Color Lerp(const Color& a, const Color& b, float t);
    static Color LerpUnclamped(const Color& a, const Color& b, float t);
    static Color Gamma(float r, float g, float b, float a = 1.0f);
    static Color Gamma(const Color& color);
    static Color GrayScale(float r, float g, float b, float a = 1.0f);
    static Color GrayScale(const Color& color);
    static Color Linear(float r, float g, float b, float a = 1.0f);
    static Color Linear(const Color& color);
    static Color MaxColor(float r, float g, float b, float a = 1.0f);
    static Color MaxColor(const Color& color);
    static Color HSVToRGB(float h, float s, float v);
    static Color RGBToHSV(float r, float g, float b);
    static Color RGBToHSV(const Color& color);
    static Color Black();
    static Color White();
    static Color Red();
    static Color Green();
    static Color Blue();
    static Color Yellow();
    static Color Cyan();
    static Color Grey();
    static Color Magenta();

public:
    void Gamma() const;
    void GrayScale() const;
    void Linear() const;
    void MaxColor() const;
    void Set(const Color& v);
    void Set(float r, float g, float b, float a);
    void Set(float rgba);
    void Set0();
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
