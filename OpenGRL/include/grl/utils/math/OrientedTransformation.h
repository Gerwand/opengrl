#pragma once

#include "Ranges.h"
#include "Vectors.h"

namespace grl {

/**
 * Simple class describing transformation of the point in the 3D space using unit
 * vector and a floating point value, which is describing the distance.
 */
class OrientedTransformation
{
public:
    /**
     * Default constructor, doing nothing.
     */
    OrientedTransformation();
    /**
     * Constructor intiializing all fields.
     *
     * @param distance Length of the transformation.
     * @param vector Vector describing orientation of the transformation.
     * @param isUnit True, if the input vector is unit vector, false otherwise.
     * If the vector is not unit, it will be normalized.
     */
    OrientedTransformation(float distance, Vec3f vector, bool isUnit = true);

    /**
     * Constructor intiializing distance and orientation of the transformation using single vector.
     * The length of the vector is used for determining distance, after that it is being normalized.
     *
     * @param vector Unit vector describing orientation of the transformation.
     */
    OrientedTransformation(Vec3f vector);

    /**
     * Get length of the transformation.
     *
     * @returns length of the transformation.
     */
    float getDistance() const;

    /**
     * Get unit vector describing orientation of the transformation.
     *
     * @returns unit vector describing orientation of the transformation.
     */
    Vec3f & getUnitVector();

    /**
     * Get unit vector describing orientation of the transformation.
     *
     * @returns unit vector describing orientation of the transformation.
     */
    const Vec3f & getUnitVector() const;

    /**
     * Get transformation vector. It is unit vector multiplied by distance.
     *
     * @returns transformation vector.
     */
    Vec3f getTransformationVector() const;

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
    assert((isUnit ? absBetween(vector.length() - 1.0f, epsilon) : true) &&
           "isUnit is true, but vector is not normalized");
    _distance = distance;
    _unitVector = isUnit ? vector : Vec3f::normalize(vector);
}

inline
OrientedTransformation::OrientedTransformation(Vec3f vector)
{
    _distance = vector.length();
    _unitVector = Vec3f::normalize(vector);
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
}
