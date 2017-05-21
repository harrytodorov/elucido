//
// Created by Haralambi Todorov on 19/05/2017.
//

#ifndef ELUCIDO_SPHERE_H
#define ELUCIDO_SPHERE_H

#define kEpsilon 0.000001

#include <glm/geometric.hpp>
#include "Object.h"

class Sphere : public Object {
public:
    float_t radius;
    glm::vec3 center;

    Sphere() :
            Object(),
            radius(1.0),
            center(glm::vec3(0))
    {}

    Sphere(const glm::dvec3 &c, const float_t &r) : Object(), radius(r), center(c) {}

    bool intersect(Ray &r, float_t &t);
};


#endif //ELUCIDO_SPHERE_H
