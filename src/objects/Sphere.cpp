//
// Created by Haralambi Todorov on 19/05/2017.
//

#include <cstdio>
#include <glm/gtc/matrix_transform.hpp>
#include "Sphere.h"

bool Sphere::intersect(const Ray &r, float_t &t, glm::vec4 &p_hit, uint32_t &ti) {
    // variable to hold distance between ray's origin and intersection point(s)
    float_t t0, t1;

    // compute the vector l, between the sphere's center c and the ray's origin o
    glm::vec4 l = c - r.get_origin();

    // compute the projection of l, s, on the ray's direction (d)
    float_t s = glm::dot(l, r.get_direction());

    // compute the squared length of the vector l, l2
    float_t l2 = glm::dot(l, l);

//    // if the l2 > r2 (origin of the ray is outside the sphere's center) and
//    // s < 0, the projection of l on d is less than 0 (the ray's origin is behind
//    // the sphere), we can reject that there is an interesection between the ray and
//    // the sphere
//    if (s < 0 && l2 > r2) return false;

    // compute the side m2, of a right triangle formed by s, l and m
    // using the Pythagorean theorem: m2 = l2 - s2
    float_t m2 = l2 - s*s;

    // if m2 > r2, the ray definitely misses the sphere
    if (m2 > r2) return false;

    // find the side, q, of the right triangle formed by r (sphere's radius),
    // m and q: q2 = r2 - m2; we want q, so q = sqrt(r2 - m2)
    float_t q = sqrtf(r2 - m2);

    // find out the distances to the 2 intersection points
    t0 = s - q;
    t1 = s + q;

    if (t0 > t1)
        std::swap(t0, t1);

    // it t0 is less than 0, take t1 instead
    if (t0 < 0) {
        // let's use t1
        t0 = t1;

        // if both of them are less than 0, terminate
        if (t0 < 0)
            return false;
    }

    // if t0 is greater than the current nearest distance to an object, terminate
    if (t0 > t)
        return false;

    // the ray's origin is within the sphere, there is only one intersection point:
    // p = r.o + (s+q) * r.d
    t = t0;

    // the hit point is then equals to: p = r.o + t*r.d
    p_hit = r.get_origin() + t * r.get_direction();

    // because we don't have a triangulated mesh, we give the triangle index value of -1
    ti = -1;

    return true;
}

void
Sphere::get_surface_properties(const glm::vec4 &hit_point, const glm::vec4 &view_direction, const uint32_t &triangle_index,
                               glm::vec4 &hit_normal) {
    // the sphere's normal is the normalized vector between the hit point and
    // the sphere's center
    hit_normal = glm::normalize(hit_point - c);
}

void Sphere::apply_camera_transformation(glm::mat4 &t) {
    c = t * c;
    bb.reset();
    bb.extend_by(glm::vec4(c.x-r, c.y-r, c.z-r, 1));
    bb.extend_by(glm::vec4(c.x+r, c.y+r, c.z+r, 1));
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

    // assign the translation matrix to the object's model transform matrix
    glm::mat4 tm = glm::translate(glm::mat4(1), tv);
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

    // assign the rotation matrix to the object's model transform matrix
    glm::mat4 rm = glm::rotate(glm::mat4(1), glm::radians(angle_of_rotation), rv);
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

    // assign the scale matrix to the object's model transform
    glm::mat4 sm = glm::scale(glm::mat4(1), sv);
    mt = sm * mt;

    // multiply the r by the scaling factor
    r *= scaling_factor;

    // update the squared radius of the sphere
    r2 = powf(r, 2.f);
}

void Sphere::set_radius(const float_t &r) {
    this->r = r;
    this->r2 = powf(r, 2.f);
    bb.reset();
    bb.extend_by(glm::vec4(c.x-r, c.y-r, c.z-r, 1));
    bb.extend_by(glm::vec4(c.x+r, c.y+r, c.z+r, 1));
}

void Sphere::set_center_p(const glm::vec4 &p) {
    this->c = p;
    bb.reset();
    bb.extend_by(glm::vec4(c.x-r, c.y-r, c.z-r, 1));
    bb.extend_by(glm::vec4(c.x+r, c.y+r, c.z+r, 1));
}

void Sphere::apply_transformations() {
    // apply the transformations stored in the sphere's model transform matrix to its position
    c = mt * c;
    bb.reset();
    bb.extend_by(glm::vec4(c.x-r, c.y-r, c.z-r, 1));
    bb.extend_by(glm::vec4(c.x+r, c.y+r, c.z+r, 1));

    // after applying the transformations to a sphere; its model transform matrix is set back to the identity matrix
    mt = glm::mat4(1);
}