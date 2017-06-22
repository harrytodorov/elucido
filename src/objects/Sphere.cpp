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
    glm::vec3 tmv(0);

    switch (axes_of_translation) {
        case X :
            tmv.x = translation;
            break;
        case Y :
            tmv.y = translation;
            break;
        case Z :
            tmv.z = translation;
            break;
        case XY :
            tmv.x = translation;
            tmv.y = translation;
            break;
        case XZ :
            tmv.x = translation;
            tmv.z = translation;
            break;
        case YZ :
            tmv.y = translation;
            tmv.z = translation;
            break;
        case XYZ :
            tmv = glm::vec3(translation);
            break;
        default:
            printf("You're using an undefined axis of translation.");
            break;
    }

    // get the translation matrix
    glm::mat4 tmp_m = glm::translate(glm::mat4(1), tmv);

    // assign the translation matrix to the object's model transform matrix
    mt = tmp_m * mt;

    // apply the translation to the sphere's center
    center = tmp_m * center;

}

void Sphere::rotate(const float_t &angle_of_rotation, const uint32_t &axes_of_rotation) {
    // well...sphere is defined using parametric equation, so it has just a center and a radius
    // not much sense in rotating a perfect sphere...EXCEPT, when there is a texture applied to the sphere

    // create 3d vector to determine the axis of rotation
    glm::vec3 tmv(0);

    switch (axes_of_rotation) {
        case X :
            tmv.x = 1;
            break;
        case Y :
            tmv.y = 1;
            break;
        case Z :
            tmv.z = 1;
            break;
        case XY :
            tmv.x = 1;
            tmv.y = 1;
            break;
        case XZ :
            tmv.x = 1;
            tmv.z = 1;
            break;
        case YZ :
            tmv.y = 1;
            tmv.z = 1;
            break;
        case XYZ :
            tmv = glm::vec3(1);
            break;
        default:
            printf("You're using an undefined axis of rotation.");
            break;
    }

    // get the rotation matrix
    glm::mat4 tmp_r = glm::rotate(glm::mat4(1), angle_of_rotation, tmv);

    // assign the rotation matrix to the object's model transform matrix
    mt = tmp_r * mt;

    // apply the rotation to the sphere's center
    center = tmp_r * center;
}

void Sphere::scale(const float_t &scaling_factor, const uint32_t &axes_of_scale) {
    // create 3d vector to determine the axes of scale
    glm::vec3 tmv(0);

    switch (axes_of_scale) {
        case X :
            tmv.x = scaling_factor;
            break;
        case Y :
            tmv.y = scaling_factor;
            break;
        case Z :
            tmv.z = scaling_factor;
            break;
        case XY :
            tmv.x = scaling_factor;
            tmv.y = scaling_factor;
            break;
        case XZ :
            tmv.x = scaling_factor;
            tmv.z = scaling_factor;
            break;
        case YZ :
            tmv.y = scaling_factor;
            tmv.z = scaling_factor;
            break;
        case XYZ :
            tmv = glm::vec3(scaling_factor);
            break;
        default:
            printf("You're using an undefined axis of scale.");
            break;
    }

    // get the scaling matrix
    glm::mat4 tmp_sm = glm::scale(glm::mat4(1), tmv);

    // assign the scale matrix to the object's model transform
    mt = tmp_sm * mt;

    // multiply the radius by the scaling factor
    radius *= scaling_factor; 
}
