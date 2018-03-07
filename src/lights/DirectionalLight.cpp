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
    glm::vec3 tv = create_transformation_vector(axes_of_translation,
                                                translation);

    // assign the translation matrix to the light's model transform
    glm::mat4 tm = glm::translate(glm::mat4(1), tv);
    mt = tm * mt;
}

void DirectionalLight::rotate(const float_t &angle_of_rotation, const Axis &axes_of_rotation) {
    glm::vec3 rv = create_transformation_vector(axes_of_rotation,
                                                angle_of_rotation);

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