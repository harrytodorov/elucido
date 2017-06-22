//
// Created by Haralambi Todorov on 19/05/2017.
//

#include <cstdio>
#include <glm/gtc/matrix_transform.hpp>
#include "Sphere.h"

bool Sphere::intersect(Ray &r, float_t &t, glm::vec4 &p_hit, glm::vec4 &hit_norm) {
    glm::vec4 temp = r.orig - center;
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
        hit_norm = ((float_t) (1.0/radius)) * glm::vec4(p_hit - center);

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

void Sphere::apply_transformation(glm::mat4 &t) {
    center = t * center;
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

    // apply the translation to the sphere's center
    center = tm * center;

}

void Sphere::rotate(const float_t &angle_of_rotation, const uint32_t &axes_of_rotation) {
    // well...sphere is defined using parametric equation, so it has just a center and a radius
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

    // apply the rotation to the sphere's center
    center = rm * center;
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

    // multiply the radius by the scaling factor
    radius *= scaling_factor;
}
