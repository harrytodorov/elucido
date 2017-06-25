//
// Created by Haralambi Todorov on 19/05/2017.
//

#include <cstdio>
#include <glm/gtc/matrix_transform.hpp>
#include "Sphere.h"

bool Sphere::intersect(const Ray &r, float_t &t, glm::vec4 &p_hit) {
    // compute the vector l, between the sphere's center c and the ray's origin o
    glm::vec4 l = c - r.o;

    // compute the projection of l, s, on the ray's direction (d)
    float_t s = glm::dot(l, r.d);

    // compute the squared length of the vector l, l2
    float_t l2 = glm::dot(l, l);

    // if the l2 > r2 (origin of the ray is outside the sphere's center) and
    // s < 0, the projection of l on d is less than 0 (the ray's origin is behind
    // the sphere), we can reject that there is an interesection between the ray and
    // the sphere
    if (s < 0 && l2 > r2) return false;

    // compute the side m2, of a right triangle formed by s, l and m
    // using the Pythagorean theorem: m2 = l2 - s2
    float_t m2 = l2 - s*s;

    // if m2 > r2, the ray definitely misses the sphere
    if (m2 > r2) return false;

    // find the side, q, of the right triangle formed by r (sphere's radius),
    // m and q: q2 = r2 - m2; we want q, so q = sqrt(r2 - m2)
    float_t q = sqrtf(r2 - m2);

    // if l2 > r2, there are 2 intersection points with the ray and the one we need is
    // the closer one: p = r.o + (s-q) * r.d
    if (l2 > r2) {
        // t = s-q, the closest distance between the ray's origin and an ray-sphere intersection
        t = s - q;

        // the hit point is then equals to: p = r.o + t*r.d
        p_hit = r.o + t * r.d;

        return true;
    }

    // the ray's origin is within the sphere, there is only one intersection point:
    // p = r.o + (s+q) * r.d
    t = s + q;

    // the hit point is then equals to: p = r.o + t*r.d
    p_hit = r.o + t * r.d;

    return true;

}

void
Sphere::get_surface_properties(const glm::vec4 &hit_point, const glm::vec4 &view_direction, glm::vec4 &hit_normal) {
    // the sphere's normal is the normalized vector between the hit point and
    // the sphere's center
    hit_normal = glm::normalize(hit_point - c);
}


void Sphere::apply_camera_transformation(glm::mat4 &t) {
    c = t * c;
}

void Sphere::translate(const float_t &translation, const uint32_t &axes_of_translation) {
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

    // assign the translation matrix to the object's model transform matrix
    mt = tm * mt;
}

void Sphere::rotate(const float_t &angle_of_rotation, const uint32_t &axes_of_rotation) {
    // well...sphere is defined using parametric equation, so it has just a c and a r
    // not much sense in rotating a perfect sphere...EXCEPT, when there is a texture applied to the sphere

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

    // assign the rotation matrix to the object's model transform matrix
    mt = rm * mt;
}

void Sphere::scale(const float_t &scaling_factor, const uint32_t &axes_of_scale) {
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

    // multiply the r by the scaling factor
    r *= scaling_factor;

    // update the squared radius of the sphere
    r2 = powf(r, 2.f);
}

void Sphere::set_radius(const float_t &r) {
    this->r = r;
    this->r2 = powf(r, 2.f);
}

void Sphere::set_center_p(const glm::vec4 &p) {
    this->c = p;
}

void Sphere::apply_transformations() {
    // apply the transformations stored in the sphere's model transform matrix to its position
    c = mt * c;
}