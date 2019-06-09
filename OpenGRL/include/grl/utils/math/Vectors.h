#pragma once

#include <string>
#include <iomanip>

#ifndef OPENCV_SKIP
#include <opencv2/core/core.hpp>
#endif

namespace grl {

constexpr int vecToStrPrecission = 2;
//////////////////////////////////////////////////
// Vector2
//////////////////////////////////////////////////

/**
 * Class representing 2D vector.
 *
 * @tparam T the type of data in the vector.
 */
template <typename T>
struct Vector2
{
    // Represent vector as the array or the fields for convenience.
    union {
        struct {
            T x;
            T y;
        };
        T coords[2];
    };

    /**
     * Default constructor doing nothing.
     */
    Vector2() {}

    /**
     * Constructor initializing all fields of the vector.
     *
     * @param x Coordinate x.
     * @param y Coordinate y.
     */
    Vector2(T x, T y) : coords{ x, y } {}

    /**
     * Calculate Euclidean length of the vector.
     *
     * @returns Euclidean length of the vector.
     */
    float length() const;

#ifndef OPENCV_SKIP
    /**
     * Convert vector to the OpenCV Point class treating this 2D vector as a point.
     *
     * @returns OpenCV Point object with x and y the same as in the vector.
     */
    operator cv::Point() const { return cv::Point(static_cast<int>(x), static_cast<int>(y)); }
#endif
};

typedef Vector2<int> Vec2i;
typedef Vector2<float> Vec2f;

template<typename T>
float Vector2<T>::length() const
{
    return sqrtf(static_cast<float>(x*x + y*y));
}

//////////////////////////////////////////////////
// Vector3
//////////////////////////////////////////////////

/**
 * Class representing 3D vector.
 *
 * @tparam T the type of data in the vector.
 */
template <typename T>
struct Vector3
{
    // Access members as an array or fields for convenience.
    union {
        struct {
            T x;
            T y;
            T z;
        };
        T coords[3];
    };

    /**
     * Default constructor doing nothing.
     */
    Vector3() {}

    /**
     * Constructor initializing all fields of the vector.
     *
     * @param x Coordinate x.
     * @param y Coordinate y.
     * @param z Coordinate z.
     */
    Vector3(T x, T y, T z) : coords{ x, y, z } {}

    /**
     * Dot (scalar) product of two vectors.
     *
     * @param vec3 the cons Vector3 which should be used for calculating dot product.
     * @returns dot product of two vectors.
     */
    T dot(const Vector3 &vec3) { return x*vec3.x + y*vec3.y + z*vec3.z; }

    /**
     * Cross (vector) product of two vectors.
     * It creates orthogonal vector to the plane defined by two input vectors.
     *
     * @param vec3 the cons Vector3 which should be used for calculating cross product.
     * @returns cross product of two vectors.
     */
    Vector3 cross(const Vector3 &vec3)
    {
        return Vector3{
            coords[1]*vec3[2] - coords[2]*vec3[1],
            coords[2]*vec3[0] - coords[0]*vec3[2],
            coords[0]*vec3[1] - coords[1]*vec3[0]
        };
    }

    /**
     * Calculate Euclidean length of the vector.
     *
     * @returns Euclidean length of the vector.
     */
    float length() const;

    /**
     * Normalize vector in place.
     */
    void normalize();

    /**
     * Calculate and return normalized vector from the input one.
     *
     * @param vec Vector, which should be normalized.
     * @returns normalized vector.
     */
    static Vector3 normalize(Vector3 vec);

    /**
     * Calculate absolute Euclidean distance from two points (vectors).
     *
     * @param vec1 First vector (point).
     * @param vec2 Second vector (point).
     * @returns Absolute Euclidean distance from two vectors (points)
     */
    static float distance(Vector3<T> vec1, Vector3<T> vec2);

    /**
     * Vector can be treated as an array for convenience [x, y, z].
     */
    T &operator[](int i) { return coords[i]; }
    /**
     * Vector can be treated as an array for convenience [x, y, z].
     */
    T operator[](int i) const { return coords[i]; }

    /**
     * Convert vector to std::string. The format of the string describing vector
     * is like below:
     * <x, y, z>
     *
     * @returns string describing vector.
     */
    operator std::string() const;
};

typedef Vector3<int> Vec3i;
typedef Vector3<float> Vec3f;
typedef Vector3<char> Vec3c;

template <typename T>
Vector3<T>::operator std::string() const
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(vecToStrPrecission) <<
        '<' << x <<
        ',' << y <<
        ',' << z <<
        '>';

    return ss.str();
}

template <typename T>
float
Vector3<T>::length() const 
{
    return sqrtf(static_cast<float>(x*x + y*y + z*z)); 
}

template <typename T>
void
Vector3<T>::normalize()
{
    T len = length();
    (*this) /= len;
}

template <typename T>
Vector3<T>
Vector3<T>::normalize(Vector3<T> vec)
{
    T len = vec.length();
    return vec/len;
}

template <typename T>
float
Vector3<T>::distance(Vector3<T> vec1, Vector3<T> vec2)
{
    Vector3<T> diff = vec2 - vec1;
    T len = diff.length();
    return len;
}

//////////////////////////////////////////////////
// Vector3 - operators
//////////////////////////////////////////////////

template <typename T>
std::ostream & operator<<(std::ostream &out, const Vector3<T> vec)
{
    return out << std::string(vec);
}

/**
 * Operator for adding two vectors. Each coordinate will be added to the corresponding one
 * in the other vector, so x is added to x, y to y, z to z.
 *
 * @tparam T the type of data in the vector.
 * @param v1 First vector to be added.
 * @param v2 Second vector to be added.
 * @returns sum of the two input vectors.
 */
template <typename T>
Vector3<T>
operator+(Vector3<T> v1, Vector3<T> v2)
{
    return Vector3<T>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

/**
 * Operator for adding two vectors. Each coordinate will be added to the corresponding one
 * in the other vector, so x is added to x, y to y, z to z. The result will be added
 * in place, replacing value in the first input vector.
 *
 * @tparam T the type of data in the vector.
 * @param v1 First vector to which the second will be added
 * @param v2 Second vector to be added.
 * @returns reference to the modified (transformed) vector.
 */
template <typename T>
const Vector3<T> &
operator+=(Vector3<T>& v1, Vector3<T> v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;
    return v1;
}

/**
 * Operator for subtracting two vectors. Each coordinate will be subtracted from the corresponding one
 * in the other vector, so x is subtracted from x, y from y, z from z.
 *
 * @tparam T the type of data in the vector.
 * @param v1 First vector from which the second will be subtracted.
 * @param v2 Vector, that must be subtracted.
 * @returns Second vector subtracted from first vector.
 */
template <typename T>
Vector3<T>
operator-(Vector3<T> v1, Vector3<T> v2)
{
    return Vector3<T>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

/**
 * Operator for subtracting two vectors. Each coordinate will be subtracted from the corresponding one
 * in the other vector, so x is subtracted from x, y from y, z from z.
 * The result will be subtracted in place, modyfing the first vector.
 *
 * @tparam T the type of data in the vector.
 * @param v1 First vector from which the second will be subtracted.
 * @param v2 Vector, that must be subtracted.
 * @returns Reference to first vector, from which the second was subtracted.
 */
template <typename T>
const Vector3<T> &
operator-=(Vector3<T>& v1, Vector3<T> v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;
    v1.z -= v2.z;
    return v1;
}

/**
 * Operator for dividing every coordinate in the first vector by given value.
 *
 * @tparam T the type of data in the vector.
 * @param v1 Divident vector.
 * @param val Divider.
 * @returns vector's coordinates divided by value coordinates.
 */
template <typename T>
Vector3<T>
operator/(Vector3<T> v1, T val)
{
    return Vector3<T>(v1.x/val, v1.y/val, v1.z/val);
}

/**
 * Operator for dividing every coordinate in the first vector by given value.
 * The result will be divided in place, modyfing the first vector.
 *
 * @tparam T the type of data in the vector.
 * @param v1 Divident vector.
 * @param val Divider.
 * @returns Reference to divident vector.
 */
template <typename T>
Vector3<T> &
operator/=(Vector3<T> &v1, T val)
{
    v1.x /= val;
    v1.y /= val;
    v1.z /= val;
    return v1;
}

/**
 * Multiply each value in the vector by the given value.
 *
 * @tparam T the type of data in the vector.
 * @param v1 Vector, which values will be multiplied.
 * @param val Multiplier.
 * @returns Vector multiplied by the input value.
 */
template <typename T>
Vector3<T>
operator*(Vector3<T> v1, T val)
{
    return Vector3<T>(v1.x*val, v1.y*val, v1.z*val);
}

/**
 * Multiply each value in the vector by the given value.
 *
 * @tparam T the type of data in the vector.
 * @param val Multiplier.
 * @param v1 Vector, which values will be multiplied.
 * @returns Vector multiplied by the input value.
 */
template <typename T>
Vector3<T>
operator*(T val, Vector3<T> v1)
{
    return Vector3<T>(v1.x*val, v1.y*val, v1.z*val);
}

/**
 * Multiply each value in the vector by the given value.
 * The result will be mutliplied in place, modyfing the first vector.
 *
 * @tparam T the type of data in the vector.
 * @param val Multiplier.
 * @param v1 Vector, which values will be multiplied.
 * @returns Reference to multiplied vector.
 */
template <typename T>
Vector3<T> &
operator*=(Vector3<T> &v1, T val)
{
    v1.x *= val;
    v1.y *= val;
    v1.z *= val;
    return v1;
}

}
