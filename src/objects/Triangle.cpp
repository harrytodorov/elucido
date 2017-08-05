//
// Created by Haralambi Todorov on 25/05/2017.
//

#include "Triangle.h"

bool Triangle::intersect(const Ray &r, isect_info &i) {

    // check if ray is parallel to triangle; compute the dot product
    // of the triangle's normal and the ray direction
    // if ray and triangle's normal are close to 0, they don't intersect
    float_t nomal_ray_d_prod = glm::dot(normal, r.dir());
    if (fabs(nomal_ray_d_prod) < kEpsilon) return false;

    // compute the dot product of the vector between one of the triangle's vertices and
    // the ray's origin and the triangle's normal
    float_t t_nominator = glm::dot(v0 - r.orig(), normal);

    // compute the distance between the ray's origin and the hit point
    // with the triangle
    float_t t = t_nominator / nomal_ray_d_prod;

    // if the computed distance is negative, the triangle is behind the ray's origin
    if (t < kEpsilon) return false;

    // compute the intersection point
    glm::vec4 ip = r.orig() + t * r.dir();

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

    // test passed; assign variables
    i.tn = t;
    i.ip = ip;
    i.ti = (uint32_t) -1;

    return true;
}

void Triangle::get_surface_properties(isect_info &i) const {
    i.ipn = normal;
}

void Triangle::apply_camera_transformation(const glm::mat4 &ictm, const glm::mat4 &itictm) {
    v0 = ictm*v0;
    v1 = ictm*v1;
    v2 = ictm*v2;
    reshape_bb();

//    // for normals we don't use the matrix with which we transform vertices and vectors
//    // but use the transpose of the inverse of the matrix we have
//    // (proof why:
//    // https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/geometry/transforming-normals)
//    glm::mat4 nm = glm::transpose(glm::inverse(t));
//    normal = glm::normalize(nm * normal);
    normal = glm::normalize(glm::vec4(glm::cross(glm::vec3(v1)-glm::vec3(v0), glm::vec3(v2)-glm::vec3(v0)), 0));
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

    // assign the translation matrix to the object's model transform
    glm::mat4 tm = glm::translate(glm::mat4(1), tv);
    mt = tm * mt;
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
    mt = rm * mt;
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

    // assign the scale matrix to the object's model transform
    glm::mat4 sm = glm::scale(glm::mat4(1), sv);
    mt = sm * mt;
}

void Triangle::apply_transformations() {
    // apply transformations stored in the triangle's model transform matrix to the triangle's vertices
    v0 = mt * v0;
    v1 = mt * v1;
    v2 = mt * v2;
    reshape_bb();

//    // for transforming normals we don't use the matrix with which we transform vertices and vectors
//    // but use the transpose of the inverse of the matrix we have proof why:
//    // https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/geometry/transforming-normals
//    glm::mat4 nm = glm::transpose(glm::inverse(mt));
//    normal = glm::normalize(nm * normal);
    normal = glm::normalize(glm::vec4(glm::cross(glm::vec3(v1)-glm::vec3(v0), glm::vec3(v2)-glm::vec3(v0)), 0));

    // after applying the transformations to a triangle; its model transform matrix is set back to the identity matrix
    mt = glm::mat4(1);
}
