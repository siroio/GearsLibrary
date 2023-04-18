#ifndef GEAR_COLOR_H
#define GEAR_COLOR_H

#include <cstdint>
#include <string>
#include <iosfwd>

struct Color
{
public:
    union
    {
        struct
        {
            uint8_t r, g, b, a;
        };
        uint32_t rgba[4];
    };

public:
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    Color(uint8_t rgba);

public:
    std::string ToString() const;
};

Color operator - (const Color& v);
Color operator + (const Color& v1, const Color& v2);
Color operator - (const Color& v1, const Color& v2);
Color operator * (const Color& v1, const Color& v2);
Color operator * (const Color& v1, float scale);
Color operator * (float scale, const Color& v1);
Color operator / (const Color& v1, const Color& v2);
Color operator / (const Color& v1, float scale);

Color& operator += (Color& v1, const Color& v2);
Color& operator -= (Color& v1, const Color& v2);
Color& operator *= (Color& v1, const Color& v2);
Color& operator *= (Color& v1, float scale);
Color& operator /= (Color& v1, const Color& v2);
Color& operator /= (Color& v1, float scale);

bool operator == (const Color& v1, const Color v2);
bool operator != (const Color& v1, const Color v2);

std::ostream& operator << (std::ostream& stream, const Color& v);

#endif // !GEAR_COLOR_H
