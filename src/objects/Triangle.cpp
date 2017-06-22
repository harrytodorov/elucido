//
// Created by Haralambi Todorov on 25/05/2017.
//

#include <cmath>
#include <glm/glm.hpp>
#include "../Ray.h"
#include "Triangle.h"

bool Triangle::intersect(Ray &r, float_t &t, glm::vec4 &p_hit, glm::vec4 &hit_norm) {

    // check if ray is parallel to triangle
    float_t t_denominator = glm::dot(normal, r.dir);
    if (fabs(t_denominator) < kEpsilon) return false;

    // compute nominator for t
    float_t t_nominator = glm::dot(v0 - r.orig, normal);

    // compute t
    float_t t_tmp = t_nominator / t_denominator;
    if (t_tmp < kEpsilon) return false;

    // compute the intersection point
    glm::vec4 ip = r.orig + t_tmp*r.dir;

    // check if intersection point is within the defined triangle
    glm::vec4 perp_vec;
    float_t d_prod;

    // edge 0
    glm::vec4 edge0(v1 - v0);
    glm::vec4 vp0(ip - v0);
    perp_vec = glm::vec4(glm::cross(glm::vec3(edge0), glm::vec3(vp0)), 0);
    d_prod = glm::dot(normal, perp_vec);
    if (d_prod < 0) return false;

    // edge 1
    glm::vec4 edge1(v2 - v1);
    glm::vec4 vp1(ip - v1);
    perp_vec = glm::vec4(glm::cross(glm::vec3(edge1), glm::vec3(vp1)), 0);
    d_prod = glm::dot(normal, perp_vec);
    if (d_prod < 0) return false;

    // edge 2
    glm::vec4 edge2(v0 - v2);
    glm::vec4 vp2(ip - v2);
    perp_vec = glm::vec4(glm::cross(glm::vec3(edge2), glm::vec3(vp2)), 0);
    d_prod = glm::dot(normal, perp_vec);
    if (d_prod < 0) return false;

    // test passed; assign variable
    t = t_tmp;
    p_hit = ip;
    hit_norm = normal;

    return true;
}

void Triangle::apply_transformation(glm::mat4 &t) {
    v0 = t*v0;
    v1 = t*v1;
    v2 = t*v2;
    normal = glm::normalize(t*normal);
}

void Triangle::translate(const float_t &translation, const uint32_t &axes_of_translation) {

}

void Triangle::rotate(const float_t &angle_of_rotation, const uint32_t &axes_of_rotation) {

}

void Triangle::scale(const float_t &scaling_factor, const uint32_t &axes_of_scale) {

}
