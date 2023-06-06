#include <Color.h>
#include <Mathf.h>
#include <sstream>

Color::Color() : rgba{ 0.0f }
{}

Color::Color(float r, float g, float b, float a = 1.0f) : r{ r }, g{ g }, b{ b }, a{ a }
{}

Color::Color(float rgba) : rgba{ rgba }
{}

Color Color::Lerp(const Color& a, const Color& b, float t)
{
    return LerpUnclamped(a, b, Mathf::Clamp01(t));
}

Color Color::LerpUnclamped(const Color& a, const Color& b, float t)
{
    return a + ((b - a) * t);
}

void Color::Set(const Color& v)
{
    r = v.r;
    g = v.g;
    b = v.b;
    a = v.a;
}

void Color::Set(float r, float g, float b, float a)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

void Color::Set(float rgba)
{
    r = g = b = a = rgba;
}

void Color::Set0()
{
    r = 0;
    g = 0;
    b = 0;
    a = 0;
}

std::string Color::ToString() const
{
    std::ostringstream ss;
    ss << r << ", ";
    ss << g << ", ";
    ss << b << ", ";
    ss << a;

    return ss.str();
}

void Color::operator=(const Color& c)
{
    Set(c);
}

float& Color::operator[](const size_t index)
{
    if (index > 3)
    {
        throw std::out_of_range("Index is out of range");
    }
    return rgba[index];
}

Color& operator+=(Color& c1, const Color& c2)
{
    c1.r += c2.r;
    c1.g += c2.g;
    c1.b += c2.b;
    c1.a += c2.a;
    return c1;
}

Color& operator-=(Color& c1, const Color& c2)
{
    c1.r -= c2.r;
    c1.g -= c2.g;
    c1.b -= c2.b;
    c1.a -= c2.a;
    return c1;
}

Color& operator*=(Color& c1, const Color& c2)
{
    c1.r *= c2.r;
    c1.g *= c2.g;
    c1.b *= c2.b;
    c1.a *= c2.a;
    return c1;
}

Color& operator*=(Color& color, float scaler)
{
    color.r *= scaler;
    color.g *= scaler;
    color.b *= scaler;
    color.a *= scaler;
    return color;
}

Color& operator*=(float scaler, Color& color)
{
    color.r *= scaler;
    color.g *= scaler;
    color.b *= scaler;
    color.a *= scaler;
    return color;
}

Color& operator/=(Color& color, float scaler)
{
    color.r /= scaler;
    color.g /= scaler;
    color.b /= scaler;
    color.a /= scaler;
    return color;
}

Color operator+(Color& c1, const Color& c2)
{
    return Color{
        c1.r + c2.r,
        c1.g + c2.g,
        c1.b + c2.b,
        c1.a + c2.a
    };
}

Color operator-(const Color& c1, const Color& c2)
{
    return Color{
        c1.r - c2.r,
        c1.g - c2.g,
        c1.b - c2.b,
        c1.a - c2.a
    };
}

Color operator*(const Color& c1, const Color& c2)
{
    return Color{
        c1.r * c2.r,
        c1.g * c2.g,
        c1.b * c2.b,
        c1.a * c2.a
    };
}

Color operator*(const Color& color, float scaler)
{
    return Color{
        color.r * scaler,
        color.g * scaler,
        color.b * scaler,
        color.a * scaler
    };
}

Color operator*(float scaler, const Color& color)
{
    return Color{
        color.r * scaler,
        color.g * scaler,
        color.b * scaler,
        color.a * scaler
    };
}

Color operator/(const Color& color, float scaler)
{
    return Color{
        color.r / scaler,
        color.g / scaler,
        color.b / scaler,
        color.a / scaler
    };
}

std::ostream& operator<<(std::ostream& stream, const Color& color)
{
    return stream << color.ToString();
}
