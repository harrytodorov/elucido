//
// Created by Haralambi Todorov on 26/05/2017.
//

#include <glm/geometric.hpp>
#include <glm/mat4x4.hpp>
#include <cstdio>
#include <glm/gtc/matrix_transform.hpp>
#include "PointLight.h"
#include "../Utilities.h"

float_t norm(glm::vec4 &v) {
    return (float_t) ((v.x*v.x) + (v.y*v.y) + (v.z*v.z));
}

void PointLight::illuminate(const glm::vec4 &hit_point, glm::vec4 &light_dir, glm::vec3 &light_intensity,
                            float_t distance) {
    light_dir = hit_point - pos;
    float_t r2 = norm(light_dir);
    distance = (float_t) sqrt(r2);
    light_dir.x /= distance,
    light_dir.y /= distance,
    light_dir.z /= distance;
    light_intensity = (float_t) (intensity / (4.0 * M_PI * r2)) * color;
}

void PointLight::apply_camera_inverse(glm::mat4 &t) {
    pos = t * pos;
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

    // get the translation matrix
    glm::mat4 tm = glm::translate(glm::mat4(1), tv);

    // assign the translation matrix to the light's model transform
    mt = tm * mt;

    // apply the translation to the light's position
    pos = tm * pos;
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

    // get the rotation matrix
    glm::mat4 rm = glm::rotate(glm::mat4(1), glm::radians(angle_of_rotation), rv);

    // assign the rotation matrix to object's model transform
    mt = rm * mt;

    // apply the rotation matrix to the light's position
    pos = rm * pos;
}
