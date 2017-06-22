//
// Created by Haralambi Todorov on 25/05/2017.
//

#include <cmath>
#include <glm/glm.hpp>
#include <cstdio>
#include <glm/gtc/matrix_transform.hpp>
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

    // for normals we don't use the matrix with which we transform vertices and vectors
    // but use the transpose of the inverse of the matrix we have
    // (proof why:
    // https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/geometry/transforming-normals)
    glm::mat4 nm = glm::transpose(glm::inverse(t));
    normal = glm::normalize(nm*normal);
}

void Triangle::translate(const float_t &translation, const uint32_t &axes_of_translation) {
    // create 3d vector to determine the axes of translation
    glm::vec3 tv(0);

    switch (axes_of_translation) {
        case X :
            tv.x = translation;
            break;
        case Y :
            tv.y = translation;
            break;
        case Z :
            tv.z = translation;
            break;
        case XY :
            tv.x = translation;
            tv.y = translation;
            break;
        case XZ :
            tv.x = translation;
            tv.z = translation;
            break;
        case YZ :
            tv.y = translation;
            tv.z = translation;
            break;
        case XYZ :
            tv = glm::vec3(translation);
            break;
        default:
            printf("You're using an undefined axis of translation.");
            break;
    }

    // get the translation matrix
    glm::mat4 tm = glm::translate(glm::mat4(1), tv);

    // assign the translation matrix to the object's model transform
    mt = tm * mt;

    // apply the translation to the triangle's vertices
    v0 = tm * v0;
    v1 = tm * v1;
    v2 = tm * v2;

    // for transforming normals we don't use the matrix with which we transform vertices and vectors
    // but use the transpose of the inverse of the matrix we have proof why:
    // https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/geometry/transforming-normals
    glm::mat4 nm = glm::transpose(glm::inverse(tm));
    normal = glm::normalize(nm*normal);
}

void Triangle::rotate(const float_t &angle_of_rotation, const uint32_t &axes_of_rotation) {
    // create 3d vector to determine the axis of rotation
    glm::vec3 rv(0);

    switch (axes_of_rotation) {
        case X :
            rv.x = 1;
            break;
        case Y :
            rv.y = 1;
            break;
        case Z :
            rv.z = 1;
            break;
        case XY :
            rv.x = 1;
            rv.y = 1;
            break;
        case XZ :
            rv.x = 1;
            rv.z = 1;
            break;
        case YZ :
            rv.y = 1;
            rv.z = 1;
            break;
        case XYZ :
            rv = glm::vec3(1);
            break;
        default:
            printf("You're using an undefined axis of rotation.");
            break;
    }

    // get the rotation matrix
    glm::mat4 rm = glm::rotate(glm::mat4(1), glm::radians(angle_of_rotation), rv);

    // assign the rotation matrix to object's model transform
    mt = rm * mt;

    // apply the rotation matrix to the triangle's vertices
    v0 = rm * v0;
    v1 = rm * v1;
    v2 = rm * v2;

    // for transforming normals we don't use the matrix with which we transform vertices and vectors
    // but use the transpose of the inverse of the matrix we have proof why:
    // https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/geometry/transforming-normals
    glm::mat4 nm = glm::transpose(glm::inverse(rm));
    normal = glm::normalize(nm*normal);
}

void Triangle::scale(const float_t &scaling_factor, const uint32_t &axes_of_scale) {
    // create 3d vector to determine the axes of scale
    glm::vec3 sv(0);

    switch (axes_of_scale) {
        case X :
            sv.x = scaling_factor;
            break;
        case Y :
            sv.y = scaling_factor;
            break;
        case Z :
            sv.z = scaling_factor;
            break;
        case XY :
            sv.x = scaling_factor;
            sv.y = scaling_factor;
            break;
        case XZ :
            sv.x = scaling_factor;
            sv.z = scaling_factor;
            break;
        case YZ :
            sv.y = scaling_factor;
            sv.z = scaling_factor;
            break;
        case XYZ :
            sv = glm::vec3(scaling_factor);
            break;
        default:
            printf("You're using an undefined axis of scale.");
            break;
    }

    // get the scale matrix
    glm::mat4 sm = glm::scale(glm::mat4(1), sv);

    // assign the scale matrix to the object's model transform
    mt = sm * mt;

    // apply the scale matrix to the triangle's vertices
    v0 = sm * v0;
    v1 = sm * v1;
    v2 = sm * v2;

    // for transforming normals we don't use the matrix with which we transform vertices and vectors
    // but use the transpose of the inverse of the matrix we have proof why:
    // https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/geometry/transforming-normals
    glm::mat4 nm = glm::transpose(glm::inverse(sm));
    normal = glm::normalize(nm*normal);
}
