#pragma once

#include "Vectors.h"
#include "Ranges.h"

namespace grl {

/**
 * Class representing 2D plane in the 3D space.
 */
struct Plane
{
    /**
     * Default constructor doing nothing.
     */
    Plane() {}

    /**
     * Constructor which is creating plane from normal vector and point lying on the plane.
     *
     * @param normal Vector perpendicular to the plane with length of 1, describing plane orientation.
     * @param pOnPlane Point which is lying on the plane, determining position in the 3D space.
     */
    Plane(Vec3f normal, Vec3f pOnPlane);

    /**
     * Setter of the plane, doing the same thing as constructor.
     *
     * @param normal Vector perpendicular to the plane with length of 1, describing plane orientation.
     * @param pOnPlane Point which is lying on the plane, determining position in the 3D space.
     */
    void set(Vec3f normal, Vec3f pOnPlane);

    /**
     * Check relation of the point and the plane by solving equation:
     * a(x-x0) + b(y-y0) + c(z-z0)
     * If the result is < 0, then it is behind the plane, if the result is > 0, then it is in front of plane,
     * if the result is 0, then it is right on the plane.
     *
     * @param point Point which relations must be calculated.
     * @returns solved equation of the plane for the given point.
     */
    float operator()(Vec3f point);

    /**
     * Check if the point is on the plane by solving the plane equation.
     *
     * @param point Point which must be checked.
     * @returns true, if it's on the plane, false otherwise.
     */
    bool isOnPlane(Vec3f point);

    /**
     * Create plane using three points that are supposed to be lying on it.
     *
     * @param p1 First point on the plane.
     * @param p2 Second point on the plane.
     * @param p3 Third point on the plane.
     * @returns Plane object created from the given points.
     */
    static Plane fromPoints(Vec3f p1, Vec3f p2, Vec3f p3);

    /**
     * Create plane using two vectors describing plane orientation and one point which is describing it's location.
     * The cross product of v1 x v2 is used.
     *
     * @param p0 Point on the plane.
     * @param v1 First vector describing plane orientation.
     * @param v2 Second vector describing plane orientation.
     * @returns Plane object created from the given point and vectors.
     */
    static Plane fromVectors(Vec3f p0, Vec3f v1, Vec3f v2);

    Vec3f getNormal() const;
    Vec3f getOriginPoint() const;

private:
    // Vector normal to the plane describing its orientation.
    Vec3f _normal;
    // Point lying on the plane, describing it's position in 3D space.
    Vec3f _p0;
};

inline Plane::Plane(Vec3f normal, Vec3f pOnPlane)
    : _normal(normal)
    , _p0(pOnPlane)
{
    assert(absBetween(normal.length() - 1.0f, epsilon) && "Plane must be created using normal vector");
}

inline void Plane::set(Vec3f normal, Vec3f pOnPlane)
{
    assert(absBetween(normal.length() - 1.0f, epsilon) && "Plane must be created using normal vector");
    _normal = normal;
    _p0 = pOnPlane;
};

inline float Plane::operator()(Vec3f point)
{
    Vec3f v = point - _p0;
    return _normal.dot(v);
}

inline bool Plane::isOnPlane(Vec3f point)
{
    return absBetween(operator()(point), epsilon);
}

inline Vec3f Plane::getNormal() const
{
    return _normal;
}

inline Vec3f Plane::getOriginPoint() const
{
    return _p0;
}

}
