#ifndef GEARS_COLOR_H
#define GEARS_COLOR_H

#include <iosfwd>
#include <string>
#include <array>

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

    constexpr explicit Color() : rgba{ 0.0f }
    {}

    constexpr explicit Color(float r, float g, float b, float a = 1.0f) : r{ r }, g{ g }, b{ b }, a{ a }
    {
        rgba[0] = r;
        rgba[1] = g;
        rgba[2] = b;
        rgba[3] = a;
    };

    constexpr explicit Color(float rgba) : rgba{ rgba }
    {}

public:
    static Color Lerp(const Color& a, const Color& b, float t);
    static Color LerpUnclamped(const Color& a, const Color& b, float t);

public:
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

#endif // !GEARS_COLOR_H
