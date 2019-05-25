#pragma once

#include <string>
#include <iomanip>
#include <cassert>
#include <vector>
#ifndef OPENCV_SKIP
#include <opencv2/core/core.hpp>
#endif

namespace grl {
constexpr float epsilon = 0.00001f;
constexpr int vec2StrPrecission = 2;

/**
 * Check if the value is between or in the given symmetrical range.
 *
 * @tparam T the type of input data.
 * @param val Value, which is going to be tested.
 * @param comp Positive value, which is used for checking if val is between ranges from -comp to comp.
 * @returns true if value is >= -comp and <= comp, false otherwise.
 */
template<typename T>
inline bool
absBetween(T val, T comp)
{
	static_assert(std::is_signed<T>::value, "absBetween won't work with unsigned types");
	assert(comp > static_cast<T>(0) && "absBetween Value defining range must be greater than 0");
	return val <= comp && val >= -comp;
}

/**
 * Check if the value is between or in the given range.
 *
 * @tparam T the type of input data.
 * @param val Value, which is going to be tested.
 * @param max Upper limit of the range.
 * @param min Lower limit of the range.
 * @returns true if value is <= max and >= max, false otherwise.
 */
template<typename T>
inline bool
isBetween(T val, T max, T min)
{
	assert(max > min && "isBetween max must be greater than min");
	return val <= max && val >= min;
}

template <typename T>
struct Vector2
{
	union
	{
		struct
		{
			T x;
			T y;
		};
		T coords[2];
	};

    float length() const { return sqrtf(static_cast<float>(x*x + y*y)); }
#ifndef OPENCV_SKIP
    operator cv::Point() const { return cv::Point(static_cast<int>(x), static_cast<int>(y)); }
#endif
};

typedef Vector2<int> Vec2i;
typedef Vector2<float> Vec2f;

template <typename T>
struct Vector3
{
	Vector3() {}
	Vector3(T x, T y, T z) : coords{x, y, z} {}

	T dot(const Vector3 &vec3) { return x*vec3.x + y*vec3.y + z*vec3.z; }
	Vector3 cross(const Vector3 &vec3) {
		return Vector3 {
			coords[1]*vec3[2] - coords[2]*vec3[1],
			coords[2]*vec3[0] - coords[0]*vec3[2],
			coords[0]*vec3[1] - coords[1]*vec3[0]
		};
	}
	float length() const { return sqrtf(static_cast<float>(x*x + y*y + z*z)); }

	void normalize();
	static Vector3 normalize(Vector3 vec);
	static float distance(Vector3<T> vec1, Vector3<T> vec2);

	T &operator[](int i) { return coords[i]; }
	T operator[](int i) const { return coords[i]; }

    operator std::string() const;

	union {
		struct {
			T x;
			T y;
			T z;
		};
		T coords[3];
	};
};

template <typename T>
Vector3<T>::operator std::string() const
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(vec2StrPrecission) <<
        '<' << x <<
        ',' << y <<
        ',' << z <<
        '>';

    return ss.str();
}

template <typename T>
Vector3<T>
operator+(Vector3<T> v1, Vector3<T> v2)
{
	return Vector3<T>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

template <typename T>
const Vector3<T> &
operator+=(Vector3<T>& v1, Vector3<T> v2)
{
	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;
	return v1;
}

template <typename T>
Vector3<T>
operator-(Vector3<T> v1, Vector3<T> v2)
{
	return Vector3<T>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

template <typename T>
const Vector3<T> &
operator-=(Vector3<T>& v1, Vector3<T> v2)
{
	v1.x -= v2.x;
	v1.y -= v2.y;
	v1.z -= v2.z;
	return v1;
}

template <typename T>
Vector3<T>
operator/(Vector3<T> v1, T val)
{
	return Vector3<T>(v1.x/val, v1.y/val, v1.z/val);
}

template <typename T>
Vector3<T> &
operator/=(Vector3<T> &v1, T val)
{
	v1.x /= val;
	v1.y /= val;
	v1.z /= val;
	return v1;
}

template <typename T>
Vector3<T>
operator*(Vector3<T> v1, T val)
{
	return Vector3<T>(v1.x*val, v1.y*val, v1.z*val);
}

template <typename T>
Vector3<T>
operator*(T val, Vector3<T> v1)
{
	return Vector3<T>(v1.x*val, v1.y*val, v1.z*val);
}

template <typename T>
Vector3<T> &
operator*=(Vector3<T> &v1, T val)
{
	v1.x *= val;
	v1.y *= val;
	v1.z *= val;
	return v1;
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

template <typename T>
std::ostream & operator<<(std::ostream &out, const Vector3<T> vec)
{
    return out << std::string(vec);
}

typedef Vector3<int> Vec3i;
typedef Vector3<float> Vec3f;
typedef Vector3<char> Vec3c;

struct Plane
{
	Plane() {}
	Plane(Vec3f normal, Vec3f pOnPlane) : n(normal), p0(pOnPlane) {}
	void set(Vec3f normal, Vec3f pOnPlane) { n = normal; p0 = pOnPlane; };

	float operator()(Vec3f point) { Vec3f v = point - p0; return n.dot(v); }
	bool isOnPlane(Vec3f point) { return absBetween(operator()(point), epsilon); }

	static Plane fromPoints(Vec3f p1, Vec3f p2, Vec3f p3)
	{
		Plane plane;
		Vec3f r1 = p1 - p2;
		Vec3f r2 = p3 - p2;

		Vec3f orto = r1.cross(r2);

		plane.p0 = p2;
		plane.n = Vec3f::normalize(orto);

		return plane;
	}

	static Plane fromVectors(Vec3f p0, Vec3f v1, Vec3f v2)
	{
		Plane plane;

		Vec3f orto = v2.cross(v1);

		plane.p0 = p0;
		plane.n = Vec3f::normalize(orto);

		return plane;
	}

	Vec3f n;
	Vec3f p0;
};

class OrientedTransformation
{
public:
	OrientedTransformation();
	OrientedTransformation(float distance, Vec3f vector, bool isUnit = true);
	OrientedTransformation(Vec3f vector);

	void set(float distance, Vec3f vector, bool isUnit = true);
	void set(Vec3f vector);

	float getDistance() const;
	Vec3f & getUnitVector();
	const Vec3f & getUnitVector() const;
	Vec3f getTransformationVector() const;

	void setDistance(float distance);
	void setVector(Vec3f vector, bool isUnit = true);

private:
	float _distance;
	Vec3f _unitVector;
};

inline
OrientedTransformation::OrientedTransformation()
{
	// Empty constructor
}

inline
OrientedTransformation::OrientedTransformation(float distance, Vec3f vector, bool isUnit)
{
	set(distance, vector, isUnit);
}

inline
OrientedTransformation::OrientedTransformation(Vec3f vector)
{
	set(vector);
}

inline void
OrientedTransformation::set(Vec3f vector)
{
	_distance = vector.length();
	_unitVector = Vec3f::normalize(vector);
}

inline void
OrientedTransformation::set(float distance, Vec3f vector, bool isUnit)
{
	setDistance(distance);
	setVector(vector, isUnit);
}

inline float
OrientedTransformation::getDistance() const
{
	return _distance;
}

inline Vec3f &
OrientedTransformation::getUnitVector()
{
	return _unitVector;
}

inline const Vec3f &
OrientedTransformation::getUnitVector() const
{
	return _unitVector;
}

inline Vec3f
OrientedTransformation::getTransformationVector() const
{
	return _distance * _unitVector;
}

inline void
OrientedTransformation::setDistance(float distance)
{
	_distance = distance;
}

inline void
OrientedTransformation::setVector(Vec3f vector, bool isUnit)
{
	_unitVector = isUnit ? vector : Vec3f::normalize(vector);
}

/**
 * Clamp the value, so it won't be higher than max and lower than min.
 *
 * @tparam T the type of input data.
 * @param val Value, which is going to be clamped.
 * @param min Lower limit of the value.
 * @param max Upper limit of the value.
 * @returns max if val is <= max and min, if val is <= min, val otherwise.
 */
template<typename T>
inline T clamp(T val, T min, T max)
{
	assert(max > min && "clamp max must be greated than min");
	return val < min ? min : (val > max ? max : val);
}

/**
 * Clamp the value, so it won't be lower than min.
 *
 * @tparam T the type of input data.
 * @param val Value, which is going to be clamped.
 * @param min Lower limit of the value.
 * @returns min, if val is <= min, val otherwise.
 */
template<typename T>
inline T clampMin(T val, T min)
{
	return val < min ? min : val;
}

/**
 * Clamp the value, so it won't be bigger than min.
 *
 * @tparam T the type of input data.
 * @param val Value, which is going to be clamped.
 * @param max Upper limit of the value.
 * @returns min, if val is <= min, val otherwise.
 */
template<typename T>
inline T clampMax(T val, T max)
{
	return val > max ? max : val;
}

template<typename T>
inline void
safeRelease(T& COMptr)
{
	if (COMptr != nullptr) {
		COMptr->Release();
		COMptr = nullptr;
	}
}

/**
 * Sigmoid function. Returns -1 if value is lower than 0 and 1 if it is equal or higher than 0.
 *
 * @tparam T the type of input data.
 * @param val Argument of sigmoid function.
 * @returns -1 if val < 0, 1 otherwise.
 */
template<typename T>
inline int
sigma(T val)
{
	static_assert(std::is_signed<T>::value, "sigma won't work with unsigned types");
	return val < static_cast<T>(0) ? -1 : 1;
}

class GaussianKernel {
public:
    GaussianKernel(float bandwidth, size_t size);

    float & operator()(int x, int y) { return _mask[x + y * _n]; }
    const float & operator()(int x, int y) const { return _mask[x + y * _n]; }

    size_t getSize() const { return _n; }
private:
    std::vector<float> _mask;
    size_t _n;
};

inline
GaussianKernel::GaussianKernel(float bandwidth, size_t size)
{
    // Kernel must be odd
    assert(size & 1);
    _n = size;
    _mask.reserve(size);

    int mid = static_cast<int>(_n / 2);
    for (int y = 0; y < _n; ++y) {
        for (int x = 0; x < _n; ++x) {
            Vec2i coords{ (x-mid), (y-mid) };
            float val = coords.length() / bandwidth;
            _mask.push_back(exp(-val*val));
        }
    }
}

}
