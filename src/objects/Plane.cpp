//
// Created by Haralambi Todorov on 25/05/2017.
//

#include <glm/geometric.hpp>
#include "Plane.h"
#include "../Utilities.h"

bool Plane::intersect(Ray &r, float_t &t) {
    float denom = glm::dot(normal, r.dir);

    if (denom > kEpsilon)
        return false;

    glm::vec3 cp = point - r.orig;
    float_t t_plane = glm::dot(cp, normal) / denom;

    if (t_plane > kEpsilon  && t_plane < t) {
        t = t_plane;
        return true;
    }

    return false;
}
