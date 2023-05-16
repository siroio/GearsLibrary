#include <Color.h>
#include <sstream>

Color::Color() : rgba{ 0.0f }
{}

Color::Color(float r, float g, float b, float a = 1.0f) : r{ r }, g{ g }, b{ b }, a{ a }
{}

Color::Color(float rgba) : rgba{ rgba }
{}

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

std::ostream& operator<<(std::ostream& stream, const Color& color)
{
    return stream << color.ToString();
}