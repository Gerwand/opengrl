#pragma once

namespace grl {

constexpr float epsilon = 0.00001f;
/**
 * Check if the value is between or in the given symmetrical range.
 *
 * @tparam T the type of input data.
 * @param val Value, which is going to be tested.
 * @param comp Positive value, which is used for checking if val is between
 * ranges from -comp to comp.
 * @returns true if value is >= -comp and <= comp, false otherwise.
 */
template<typename T>
inline bool absBetween(T val, T comp)
{
    static_assert(std::is_signed<T>::value, "absBetween won't work with unsigned types");
    assert(comp >= static_cast<T>(0) && "absBetween Value defining range must be greater than 0");
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
inline bool isBetween(T val, T max, T min)
{
    assert(max > min && "isBetween max must be greater than min");
    return val <= max && val >= min;
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

/**
 * Sigmoid function. Returns -1 if value is lower than 0 and 1 if it is equal or
 * higher than 0.
 *
 * @tparam T the type of input data.
 * @param val Argument of sigmoid function.
 * @returns -1 if val < 0, 1 otherwise.
 */
template<typename T>
inline int sigma(T val)
{
    static_assert(std::is_signed<T>::value, "sigma won't work with unsigned types");
    return val < static_cast<T>(0) ? -1 : 1;
}

}
