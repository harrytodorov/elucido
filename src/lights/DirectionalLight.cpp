// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "DirectionalLight.h"

void DirectionalLight::illuminate(const glm::vec4 &hit_point, glm::vec4 &light_dir, glm::vec3 &light_intensity,
                                  float_t &distance) {
    light_dir = glm::normalize(d);
    light_intensity = intensity * color;
    distance = infinity;
}

void DirectionalLight::apply_camera_transformation(const glm::mat4 &ictm, const glm::mat4 &itctm) {
    d = ictm * d;
}

void DirectionalLight::translate(const float_t &translation, const Axis &axes_of_translation) {
    // create 3d vector to determine the axes of translation
    glm::vec3 tv(0);

    switch (axes_of_translation) {
        case Axis::X :
            tv.x = translation;
            break;
        case Axis::Y :
            tv.y = translation;
            break;
        case Axis::Z :
            tv.z = translation;
            break;
        case Axis::XY :
            tv.x = translation;
            tv.y = translation;
            break;
        case Axis::XZ :
            tv.x = translation;
            tv.z = translation;
            break;
        case Axis::YZ :
            tv.y = translation;
            tv.z = translation;
            break;
        case Axis::XYZ :
            tv = glm::vec3(translation);
            break;
        default:
            printf("You're using an undefined axis of translation.");
            break;
    }

    // assign the translation matrix to the light's model transform
    glm::mat4 tm = glm::translate(glm::mat4(1), tv);
    mt = tm * mt;
}

void DirectionalLight::rotate(const float_t &angle_of_rotation, const Axis &axes_of_rotation) {
    // create 3d vector to determine the axis of rotation
    glm::vec3 rv(0);

    switch (axes_of_rotation) {
        case Axis::X :
            rv.x = 1;
            break;
        case Axis::Y :
            rv.y = 1;
            break;
        case Axis::Z :
            rv.z = 1;
            break;
        case Axis::XY :
            rv.x = 1;
            rv.y = 1;
            break;
        case Axis::XZ :
            rv.x = 1;
            rv.z = 1;
            break;
        case Axis::YZ :
            rv.y = 1;
            rv.z = 1;
            break;
        case Axis::XYZ :
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

void DirectionalLight::apply_transformations() {
    // apply the transformations stored in the light's model transform matrix to its direction and
    // normalize the direction vector
    d = glm::normalize(mt * d);

    // after applying the transformations to a directional light; its model transform matrix is set back to the identity matrix
    mt = glm::mat4(1);
}