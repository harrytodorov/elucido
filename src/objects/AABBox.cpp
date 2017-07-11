//
// Created by Haralambi Todorov on 11/07/2017.
//

#include "AABBox.h"

AABBox &AABBox::extend_by(const glm::vec4 &p) {
    // check min bound
    if (p.x < bounds[0].x) bounds[0].x = p.x;
    if (p.y < bounds[0].y) bounds[0].y = p.y;
    if (p.z < bounds[0].z) bounds[0].z = p.z;

    // check max bound
    if (p.x > bounds[1].x) bounds[1].x = p.x;
    if (p.y > bounds[1].y) bounds[1].y = p.y;
    if (p.z > bounds[1].z) bounds[1].z = p.z;

    return *this;
}

bool AABBox::intersect(const Ray &r) {
    float_t tmin, tmax, tymin, tymax, tzmin, tzmax;

    tmin = (bounds[r.get_sign()[0]].x - r.get_origin().x) * r.get_inv_direction().x;
    tmax = (bounds[1-r.get_sign()[0]].x - r.get_origin().x) * r.get_inv_direction().x;

    tymin = (bounds[r.get_sign()[1]].y - r.get_origin().y) * r.get_inv_direction().y;
    tymax = (bounds[1-r.get_sign()[1]].y - r.get_origin().y) * r.get_inv_direction().y;

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
        tmin = tymin;

    if (tymax < tmax)
        tmax = tymax;

    tzmin = (bounds[r.get_sign()[2]].z - r.get_origin().z) * r.get_inv_direction().z;
    tzmax = (bounds[1-r.get_sign()[2]].z - r.get_origin().z) * r.get_inv_direction().z;

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;

    if (tzmin > tmin)
        tmin = tzmin;

    if (tzmax < tmax)
        tmax = tzmax;

    return true;
}