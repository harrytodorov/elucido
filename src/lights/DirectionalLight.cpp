// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "DirectionalLight.h"

void DirectionalLight::illuminate(const glm::vec4 &hit_point, glm::vec4 &light_dir, glm::vec3 &light_intensity,
                                  float_t &distance) {
    light_dir = d;
    light_intensity = intensity * color;
    distance = infinity;
}

void DirectionalLight::apply_camera_transformation(const glm::mat4 &ivm) {
    d = ivm * d;
}

void DirectionalLight::translate(const float_t &translation, const Axis &axes_of_translation) {
  // One can ignore translating a directional light source.
  // The translation doesn't effect directions.
}

void DirectionalLight::rotate(const float_t &angle_of_rotation, const Axis &axes_of_rotation) {
    glm::vec3 rv = create_transformation_vector(axes_of_rotation,
                                                1);

    glm::mat4 rm = glm::rotate(glm::mat4(1),
                               glm::radians(angle_of_rotation),
                               rv);
    mt = rm * mt;
}

void DirectionalLight::apply_transformations() {
    d = glm::transpose(glm::inverse(mt)) * d;

    // Reset the model transform matrix.
    mt = glm::mat4(1);
}