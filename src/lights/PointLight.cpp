//
// Created by Haralambi Todorov on 26/05/2017.
//

#include <glm/geometric.hpp>
#include <glm/mat4x4.hpp>
#include <cstdio>
#include <glm/gtc/matrix_transform.hpp>
#include "PointLight.h"

void PointLight::illuminate(const glm::vec4 &hit_point, glm::vec4 &light_dir, glm::vec3 &light_intensity, float_t &distance) {
    float_t sq_dist;

    // compute the light direction
    light_dir = hit_point - p;

    // compute the distance between the light source and the hit point
    sq_dist = glm::dot(light_dir, light_dir);
    distance = glm::sqrt(sq_dist);

    // now we can safely normalize the light direction vector
    light_dir = glm::normalize(light_dir);

    // for computing the light intensity use the inverse square law
    light_intensity = intensity * color / (float_t) (4.f * M_PI * sq_dist);
}

void PointLight::apply_camera_transformation(glm::mat4 &t) {
    p = t * p;
}

void PointLight::translate(const float_t &translation, const uint32_t &axes_of_translation) {
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

    // assign the translation matrix to object's model transform
    glm::mat4 tm = glm::translate(glm::mat4(1), tv);
    mt = tm * mt;
}

void PointLight::rotate(const float_t &angle_of_rotation, const uint32_t &axes_of_rotation) {
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

    // assign the rotation matrix to object's model transform
    glm::mat4 rm = glm::rotate(glm::mat4(1), glm::radians(angle_of_rotation), rv);
    mt = rm * mt;
}

void PointLight::apply_transformations() {
    // apply the transformations stored in the light's model transorm matrix to its position
    p = mt * p;

    // after applying the transformations to a point light; its model transform matrix is set back to the identity matrix
    mt = glm::mat4(1);
}
