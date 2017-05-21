//
// Created by Haralambi Todorov on 19/05/2017.
//

#include "Sphere.h"

bool Sphere::intersect(Ray &r, float_t &t) {
    glm::vec3 temp = r.orig - center;
    float_t t1, t2;
    float_t a = glm::dot(r.dir, r.dir);
    float_t b = 2.0 * glm::dot(temp, r.dir);
    float_t c = glm::dot(temp, temp) - pow(radius, 2);
    float_t disc = pow(b, 2) - 4.0 * a * c;

    if (disc < 0.0)
        return false;

    double e = sqrt(disc);
    double denom = 2.0 * a;

    t1 = (-b - e); // smaller root
    if (t1 > kEpsilon) {
        t = t1;
        return true;
    }

    t2 = (-b + e); // bigger root
    if (t2 > kEpsilon) {
        t = t2;
        return true;
    }

    return false;
}
