//
// Created by Haralambi Todorov on 11/07/2017.
//

#include "AABBox.h"
#include "../Ray.h"

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

    tmin = (bounds[r.get_sign()[0]].x - r.orig().x) * r.get_inv_dir().x;
    tmax = (bounds[1-r.get_sign()[0]].x - r.orig().x) * r.get_inv_dir().x;

    tymin = (bounds[r.get_sign()[1]].y - r.orig().y) * r.get_inv_dir().y;
    tymax = (bounds[1-r.get_sign()[1]].y - r.orig().y) * r.get_inv_dir().y;

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
        tmin = tymin;

    if (tymax < tmax)
        tmax = tymax;

    tzmin = (bounds[r.get_sign()[2]].z - r.orig().z) * r.get_inv_dir().z;
    tzmax = (bounds[1-r.get_sign()[2]].z - r.orig().z) * r.get_inv_dir().z;

    return !((tmin > tzmax) || (tzmin > tmax));

//    if (tzmin > tmin)
//        tmin = tzmin;
//
//    if (tzmax < tmax)
//        tmax = tzmax;

}