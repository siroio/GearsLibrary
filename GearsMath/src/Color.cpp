#include <Color.h>
#include <Vector4.h>
#include <Mathf.h>
#include <sstream>

Color::Color(const Vector4& vector) : r{ vector.x }, g{ vector.y }, b{ vector.z }, a{ vector.w }
{}

Color::Color() : rgba{ 0.0f }
{}

Color::Color(float r, float g, float b, float a) : r{ r }, g{ g }, b{ b }, a{ a }
{}

Color::Color(float rgba) : rgba{ rgba }
{}

Color Color::Lerp(const Color& a, const Color& b, float t)
{
    return a + ((b - a) * Mathf::Clamp01(t));
}

Color Color::LerpUnclamped(const Color& a, const Color& b, float t)
{
    return a + ((b - a) * t);
}

Color Color::Gamma(float r, float g, float b, float a)
{
    return Color{
        Mathf::LinearToGammaSpace(r),
        Mathf::LinearToGammaSpace(g),
        Mathf::LinearToGammaSpace(b),
        a
    };
}

Color Color::Gamma(const Color& color)
{
    return color.Gamma();
}

Color Color::Linear(float r, float g, float b, float a)
{
    return Color{
        Mathf::GammaToLinearSpace(r),
        Mathf::GammaToLinearSpace(g),
        Mathf::GammaToLinearSpace(b),
        a
    };
}

Color Color::Linear(const Color& color)
{
    return color.Linear();
}

float Color::GrayScale(float r, float g, float b, float a)
{
    return 0.299F * r + 0.587F * g + 0.114F * b;
}

float Color::GrayScale(const Color& color)
{
    return color.GrayScale();
}

float Color::MaxColor(float r, float g, float b)
{
    return Mathf::Max(Mathf::Max(r, g), b);
}

float Color::MaxColor(const Color& color)
{
    return Mathf::Max(Mathf::Max(color.r, color.g), color.b);
}

Color Color::HSVToRGB(float h, float s, float v, bool hdr)
{
    Color result = White();
    if (s == 0.0f)
    {
        result.r = v;
        result.g = v;
        result.b = v;
    }
    else if (v == 0.0f)
    {
        result.r = 0.0f;
        result.g = 0.0f;
        result.b = 0.0f;
    }
    else
    {
        result.r = 0.0f;
        result.g = 0.0f;
        result.b = 0.0f;

        float t_S{ 0.0f };
        float t_V{ 0.0f };
        float h_to_floor{ 0.0f };

        t_S = s;
        t_V = v;
        h_to_floor = h * 6.0f;

        int temp{ Mathf::FloorToInt(h_to_floor) };
        float t{ h_to_floor - static_cast<float>(temp) };
        float var_1{ t_V * (1.0f - t_S) };
        float var_2{ t_V * (1.0f - t_S * t) };
        float var_3{ t_V * (1.0f - t_S * (1 - t)) };

        switch (temp)
        {
            case 0:
                result.r = t_V;
                result.g = var_3;
                result.b = var_1;
                break;
            case 1:
                result.r = var_2;
                result.g = t_V;
                result.b = var_1;
                break;

            case 2:
                result.r = var_1;
                result.g = t_V;
                result.b = var_3;
                break;

            case 3:
                result.r = var_1;
                result.g = var_2;
                result.b = t_V;
                break;

            case 4:
                result.r = var_3;
                result.g = var_1;
                result.b = t_V;
                break;

            case 5:
                result.r = t_V;
                result.g = var_1;
                result.b = var_2;
                break;

            case 6:
                result.r = t_V;
                result.g = var_3;
                result.b = var_1;
                break;

            case -1:
                result.r = t_V;
                result.g = var_1;
                result.b = var_2;
                break;
        }

        if (!hdr)
        {
            result.r = Mathf::Clamp(result.r, 0.0f, 1.0f);
            result.g = Mathf::Clamp(result.g, 0.0f, 1.0f);
            result.b = Mathf::Clamp(result.b, 0.0f, 1.0f);
        }
    }

    return result;
}

Color Color::HSVToRGB(const Color::HSV& color, bool hdr)
{
    const auto& [h, s, v] = color;
    return HSVToRGB(h, s, v, hdr);
}

Color::HSV Color::RGBToHSV(float r, float g, float b)
{
    float H{ 0.0f };
    float S{ 0.0f };
    float V{ 0.0f };

    if ((b > g) && (b > r)) RGBToHSVHelper(4.0f, b, r, g, H, S, V);
    else if (g > r) RGBToHSVHelper(2.0f, g, b, r, H, S, V);
    else RGBToHSVHelper(0.0f, r, g, b, H, S, V);

    return std::make_tuple(H, S, V);
}

Color::HSV Color::RGBToHSV(const Color& color)
{
    float H{ 0.0f };
    float S{ 0.0f };
    float V{ 0.0f };

    if ((color.b > color.g) && (color.b > color.r)) RGBToHSVHelper(4.0f, color.b, color.r, color.g, H, S, V);
    else if (color.g > color.r) RGBToHSVHelper(2.0f, color.g, color.b, color.r, H, S, V);
    else RGBToHSVHelper(0.0f, color.r, color.g, color.b, H, S, V);

    return std::make_tuple(H, S, V);
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

float Color::operator[](const size_t index) const
{
    if (index > 3)
    {
        throw std::out_of_range("Index is out of range");
    }
    return rgba[index];
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

Color operator+(const Color& c1, const Color& c2)
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

Color Color::Black()
{
    return Color{ 0.0f, 0.0f, 0.0f, 1.0f };
}

Color Color::White()
{
    return Color{ 1.0f, 1.0f, 1.0f, 1.0f };
}

Color Color::Red()
{
    return Color{ 1.0f, 0.0f, 0.0f, 1.0f };
}

Color Color::Green()
{
    return Color{ 0.0f, 1.0f, 0.0f, 1.0f };
}

Color Color::Blue()
{
    return Color{ 0.0f, 0.0f, 1.0f, 1.0f };
}

Color Color::Yellow()
{
    return Color{ 1.0f, 0.92f, 0.016f, 1.0f };
}

Color Color::Cyan()
{
    return Color{ 0, 1, 1, 1 };
}

Color Color::Grey()
{
    return Color{ 0.5, 0.5, 0.5, 1 };
}

Color Color::Magenta()
{
    return Color{ 1, 0, 1, 1 };
}

void Color::RGBToHSVHelper(float offset, float dominantcolor, float colorone, float colortwo, float& H, float& S, float& V)
{
    V = dominantcolor;
    if (V != 0.0f)
    {
        float small{ 0.0f };
        small = colorone < colortwo ? colortwo : colorone;

        float diff{ V - small };

        if (diff != 0.0f)
        {
            S = diff / V;
            H = offset + ((colorone - colortwo) / diff);
        }
        else
        {
            S = 0.0f;
            H = offset + (colorone - colortwo);
        }

        H /= 6.0f;
        if (H < 0.0f) H += 1.0f;
    }
    else
    {
        S = 0.0f;
        H = 0.0f;
    }
}

Color Color::Gamma() const
{
    return Color{
        Mathf::LinearToGammaSpace(r),
        Mathf::LinearToGammaSpace(g),
        Mathf::LinearToGammaSpace(b),
        a
    };
}

Color Color::Linear() const
{
    return Color{
        Mathf::GammaToLinearSpace(r),
        Mathf::GammaToLinearSpace(g),
        Mathf::GammaToLinearSpace(b),
        a
    };
}

float Color::GrayScale() const
{
    return 0.299F * r + 0.587F * g + 0.114F * b;
}

float Color::MaxColor() const
{
    return Mathf::Max(Mathf::Max(r, g), b);
}
