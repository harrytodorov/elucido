// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include <glm/ext.hpp>
#include "DirectionalLight.h"

//==============================================================================
void DirectionalLight::apply_camera_transformation(const glm::mat4 &ivm) {
  d = ivm * d;
}

//==============================================================================
void DirectionalLight::translate(const float_t &translation, const Axis &axes_of_translation) {
  // One can ignore translating a directional light source.
  // The translation doesn't effect directions.
}

//==============================================================================
void DirectionalLight::rotate(const float_t &angle_of_rotation, const Axis &rotation_axis) {
  apply_rotation(rotation_axis, angle_of_rotation, mt);
}

//==============================================================================
void DirectionalLight::apply_transformations() {
  d = mt * d;

  // Reset the model transform matrix.
  mt = glm::mat4(1);
}

//==============================================================================
glm::vec4 DirectionalLight::get_direction(const glm::vec4 &surface_point) {
  return -d;
}

//==============================================================================
float_t DirectionalLight::get_distance(const glm::vec4 &surface_point) {
  return infinity;
}
