//
// Created by Haralambi Todorov on 19/05/2017.
//

#include "Sphere.h"
#include "../Utilities.h"

bool Sphere::intersect(Ray &r, float_t &t, glm::vec3 &p_hit, glm::vec3 &hit_norm) {
    glm::vec3 temp = r.orig - center;
    float_t t1, t2;
    float_t a       = (float_t) glm::dot(r.dir, r.dir);
    float_t b       = (float_t) (2.0 * glm::dot(temp, r.dir));
    float_t c       = (float_t) (glm::dot(temp, temp) - pow(radius, 2));
    float_t disc    = (float_t) (pow(b, 2) - 4.0 * a * c);

    if (disc < 0.0)
        return false;

    float_t e       = (float_t) sqrt(disc);
    float_t denom   = (float_t) (2.0 * a);

    t1 = (-b - e); // smaller root
    if (t1 > kEpsilon && t1 < t) {
        t = t1;

        // hit point
        p_hit = r.orig + t*r.dir;

        // normal of a sphere is the normalized distance between the hit point and the sphere center
        hit_norm = p_hit - center;
        glm::normalize(hit_norm);

        return true;
    }

    t2 = (-b + e); // bigger root
    if (t2 > kEpsilon && t2 < t) {
        t = t2;

        // hit point
        p_hit = r.orig + t*r.dir;

        // normal of a sphere is the normalized distance between the hit point and the sphere center
        hit_norm = p_hit - center;
        glm::normalize(hit_norm);

        return true;
    }

    return false;
}
