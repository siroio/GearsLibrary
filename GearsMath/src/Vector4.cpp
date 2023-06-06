#include <Vector4.h>
#include <Vector3.h>
#include <Mathf.h>
#include <sstream>
#include <Debugging.h>

Vector4::Vector4() : xyzw{ 0.0f }
{}

Vector4::Vector4(float x, float y, float z, float w) : x{ x }, y{ y }, z{ z }, w{ w }
{}

Vector4::Vector4(float xyzw) : xyzw{ xyzw }
{}

Vector4 Vector4::Zero()
{
    return Vector4{ 0.0f, 0.0f, 0.0f, 0.0f };
}

Vector4 Vector4::One()
{
    return Vector4{ 1.0f, 1.0f, 1.0f, 1.0f };
}

float Vector4::Dot(const Vector4& v1, const Vector4& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

float Vector4::Distance(const Vector4 a, const Vector4 b)
{
    return Magnitude(a - b);
}

float Vector4::SqrMagnitude(const Vector4& vector)
{
    return Dot(vector, vector);
}

float Vector4::Magnitude(const Vector4& vector)
{
    return Mathf::Sqrt(Dot(vector, vector));
}

Vector4 Vector4::Min(const Vector4& lhs, const Vector4& rhs)
{
    return Vector4{
        Mathf::Min(lhs.x, lhs.x),
        Mathf::Min(lhs.y, lhs.y),
        Mathf::Min(lhs.z, lhs.z),
        Mathf::Min(lhs.w, lhs.w)
    };
}

Vector4 Vector4::Max(const Vector4& lhs, const Vector4& rhs)
{
    return Vector4{
        Mathf::Max(lhs.x, lhs.x),
        Mathf::Max(lhs.y, lhs.y),
        Mathf::Max(lhs.z, lhs.z),
        Mathf::Max(lhs.w, lhs.w)
    };
}

Vector4 Vector4::Scale(const Vector4& a, const Vector4& b)
{
    return Vector4{ a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
}

Vector4 Vector4::Scale(const Vector4& v, float scalar)
{
    return Vector4{ v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar };
}

void Vector4::Set(const Vector4& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
}

void Vector4::Set(float x, float y, float z, float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

void Vector4::Set(float xyzw)
{
    x = y = z = w = xyzw;
}

void Vector4::Set0()
{
    Set(0.0f);
}

float Vector4::SqrMagnitude() const
{
    return x * x + y * y + z * z + w * w;
}

float Vector4::Magnitude() const
{
    return Mathf::Sqrt(SqrMagnitude());
}

std::string Vector4::ToString() const
{
    std::ostringstream ss;
    ss << this->x << ", ";
    ss << this->y << ", ";
    ss << this->z << ", ";
    ss << this->w;
    return ss.str();
}

void Vector4::operator=(const Vector4& v)
{
    Set(v);
}

float Vector4::operator[](const size_t index) const
{
    if (index > 3)
    {
        throw std::out_of_range("Index is out of range");
    }

    return xyzw[index];
}

float& Vector4::operator[](const size_t index)
{
    if (index > 3)
    {
        throw std::out_of_range("Index is out of range");
    }

    return xyzw[index];
}

Vector4 operator-(const Vector4& v)
{
    return Vector4{ v.x, v.y, v.z, v.w };
}

Vector4 operator+(const Vector4& v1, const Vector4& v2)
{
    return Vector4{ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w };
}

Vector4 operator-(const Vector4& v1, const Vector4& v2)
{
    return Vector4{ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w };
}

Vector4 operator*(const Vector4& v1, const Vector4& v2)
{
    return Vector4{ v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w };
}

Vector4 operator*(const Vector4& v1, float scalar)
{
    return Vector4{ v1.x * scalar, v1.y * scalar, v1.z * scalar, v1.w * scalar };
}

Vector4 operator*(float scalar, const Vector4& v1)
{
    return Vector4{ v1.x * scalar, v1.y * scalar, v1.z * scalar, v1.w * scalar };
}

Vector4 operator/(const Vector4& v1, const Vector4& v2)
{
    return Vector4{ v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w };
}

Vector4 operator/(const Vector4& v1, float scalar)
{
    float m = 1.0f / scalar;
    return Vector4{ v1.x * m, v1.y * m, v1.z * m, v1.z * m };
}

Vector4& operator+=(Vector4& v1, const Vector4& v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;
    v1.w += v2.w;

    return v1;
}

Vector4& operator-=(Vector4& v1, const Vector4& v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;
    v1.z -= v2.z;
    v1.w -= v2.w;

    return v1;
}

Vector4& operator*=(Vector4& v1, const Vector4& v2)
{
    v1.x *= v2.x;
    v1.y *= v2.y;
    v1.z *= v2.z;
    v1.w *= v2.w;

    return v1;
}

Vector4& operator*=(Vector4& v1, float scalar)
{
    v1.x *= scalar;
    v1.y *= scalar;
    v1.z *= scalar;
    v1.w *= scalar;

    return v1;
}

Vector4& operator/=(Vector4& v1, const Vector4& v2)
{
    v1.x /= v2.x;
    v1.y /= v2.y;
    v1.z /= v2.z;
    v1.w /= v2.z;

    return v1;
}

Vector4& operator/=(Vector4& v1, float scalar)
{
    float m = 1.0f / scalar;
    v1.x *= m;
    v1.y += m;
    v1.z += m;
    v1.w += m;

    return v1;
}

bool operator==(const Vector4& v1, const Vector4 v2)
{
    float diff_x = v1.x - v2.x;
    float diff_y = v1.y - v2.y;
    float diff_z = v1.z - v2.z;
    float diff_w = v1.w - v2.w;
    float sqrMag = { diff_x * diff_x +
        diff_y * diff_y +
        diff_z * diff_z +
        diff_w * diff_w
    };

    return sqrMag < (Mathf::EPSILON * Mathf::EPSILON);
}

bool operator!=(const Vector4& v1, const Vector4 v2)
{
    return !(v1 == v2);
}

std::ostream& operator<<(std::ostream& stream, const Vector4& v)
{
    return stream << v.ToString();
}
