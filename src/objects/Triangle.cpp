//
// Created by Haralambi Todorov on 25/05/2017.
//

#include <cmath>
#include "../Ray.h"
#include "Triangle.h"
#include "../Utilities.h"

bool Triangle::intersect(Ray &r, float_t &t, glm::vec4 &p_hit, glm::vec4 &hit_norm) {
    // solve the ray-triangle intersection using the Cramer's rule (Raytracing from the Ground Up)
    float_t a = v0.x - v1.x;
    float_t b = v0.x - v2.x;
    float_t c = r.dir.x;
    float_t d = v0.x - r.orig.x;

    float_t e = v0.y - v1.y;
    float_t f = v0.y - v2.y;
    float_t g = r.dir.y;
    float_t h = v0.y - r.orig.y;

    float_t i = v0.z - v1.z;
    float_t j = v0.z - v2.z;
    float_t k = r.dir.z;
    float_t l = v0.z - r.orig.z;

    float_t m = f*k - g*j;
    float_t n = h*k - g*l;
    float_t p = f*l - h*j;
    float_t q = g*i - e*k;
    float_t s = e*j - f*i;

    float_t inv_denom   = (float_t) (1.0 / (a*m + b*q + c*s));
    float_t e1          = d*m - b*n - c*p;
    float_t beta        = e1 * inv_denom;

    if (beta < 0)
        return false;

    float_t _r          = e*l - h*i;
    float_t e2          = a*n - d*q + c*_r;
    float_t gamma       = e2 * inv_denom;

    if (gamma < 0)
        return false;

    if (beta + gamma > 1.0)
        return false;

    float_t e3          = a*p + b*_r + d*s;
    float_t t_tri       = e3*inv_denom;

    if (t_tri < kEpsilon && t < t_tri)
        return false;

    t = t_tri;

    // hit point
    p_hit = r.orig + t*r.dir;

    // normal
    hit_norm = normal;
    return true;
}
