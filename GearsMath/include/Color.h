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
        struct
        {
            std::array<float, 4> rgba;
        };
    };

public:
    Color(const Color& color) = default;
    explicit Color();
    explicit Color(float r, float g, float b, float a);
    explicit Color(float rgba);

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
