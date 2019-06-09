#include <grl/utils/math/Plane.h>

namespace grl {

Plane Plane::fromPoints(Vec3f p1, Vec3f p2, Vec3f p3)
{
    Vec3f r1 = p1 - p2;
    Vec3f r2 = p3 - p2;
    Vec3f orto = r1.cross(r2);

    return Plane(Vec3f::normalize(orto), p2);
}

Plane Plane::fromVectors(Vec3f p0, Vec3f v1, Vec3f v2)
{
    Vec3f orto = v1.cross(v2);

    return Plane(Vec3f::normalize(orto), p0);
}

}
