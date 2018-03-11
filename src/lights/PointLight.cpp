// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "PointLight.h"

//=============================================================================
void PointLight::illuminate(const glm::vec4 &hit_point,
                            glm::vec4 &light_dir,
                            glm::vec3 &light_intensity,
                            float_t &distance) {
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

//=============================================================================
void PointLight::apply_camera_transformation(const glm::mat4 &ivm) {
  p = ivm * p;
}

//=============================================================================
void PointLight::translate(const float_t &translation,
                           const Axis &translation_axis) {
  apply_translation(translation_axis, translation, mt);
}

//=============================================================================
void PointLight::rotate(const float_t &angle_of_rotation,
                        const Axis &axes_of_rotation) {
  // One can ignore rotating point light sources.
  // Rotation doesn't affect a single point in space.
}

//=============================================================================
void PointLight::apply_transformations() {
  p = mt * p;

  // Reset the model transform matrix.
  mt = glm::mat4(1);
}
