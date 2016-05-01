#ifndef VEC2_H
#define VEC2_H

#include <math.h>

namespace arrakis
{

namespace utils
{

/**
 *  A very simple implementation of a vector class.
 */
template <class T>
class vec2
{
public:
    vec2() : x(0), y(0) {}
    vec2(T x, T y) : x(x), y(y) {}
    vec2(const vec2<T> & v) = default; // default copy constructor

    vec2<T>& operator=(const vec2<T> & v)
    {
        x = v.x;
        y = v.y;
        return *this;
    }

    vec2<T>& operator+=(const vec2<T> & rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    friend vec2<T> operator+(vec2<T> lhs, const vec2<T> & rhs)
    {
        lhs += rhs;
        return lhs;
    }

    vec2<T>& operator-=(const vec2<T> & rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    friend vec2<T> operator-(vec2<T> lhs, const vec2<T> & rhs)
    {
        lhs -= rhs;
        return lhs;
    }

    vec2<T>& operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    friend vec2<T> operator*(vec2<T> lhs, float scalar)
    {
        lhs *= scalar;
        return lhs;
    }

    vec2<T>& operator/=(float scalar)
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    friend vec2<T> operator/(vec2<T> lhs, float scalar)
    {
        lhs /= scalar;
        return lhs;
    }

    bool operator==(const vec2<T> & rhs) const
    {
        return (x == rhs.x) && (y == rhs.y);
    }

    float length() const
    {
        return std::sqrt(x*x + y*y);
    }

    vec2<T>& norm()
    {
        auto _length = length();
        x /= _length;
        y /= _length;
        return *this;
    }

    T x, y;
};

template <class T>
float dot(const vec2<T> & lhs, const vec2<T> & rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

using vec2i = vec2<int>;
using vec2f = vec2<float>;

} // end utils
} // end arrakis

#endif // VEC2_H
