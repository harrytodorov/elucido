// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include <glm/glm.hpp>

#include "PointLight.h"
#include "../core/Common.h"

//==============================================================================
void PointLight::apply_camera_transformation(const glm::mat4 &ivm) {
  p = ivm * p;
}

//==============================================================================
void PointLight::translate(const float_t &translation,
                           const Axis &translation_axis) {
  apply_translation(translation_axis, translation, mt);
}

//==============================================================================
void PointLight::rotate(const float_t &angle_of_rotation,
                        const Axis &axes_of_rotation) {
  // One can ignore rotating point light sources.
  // Rotation doesn't affect a single point in space.
}

//==============================================================================
void PointLight::apply_transformations() {
  p = mt * p;

  // Reset the model transform matrix.
  mt = glm::mat4(1);
}

//==============================================================================
glm::vec4 PointLight::get_direction(const glm::vec4 &surface_point) {
  auto direction = p - surface_point;
  // Make sure that the w-component of the direction vector is 0,
  // before normalizing. Just a safety measure.
  direction.w = 0.f;
  return glm::normalize(direction);
}

//==============================================================================
float_t PointLight::get_distance(const glm::vec4 &surface_point) {
  auto distance_vector = p - surface_point;
  auto distance = glm::length(distance_vector);
  return static_cast<float_t>(distance);
}

//==============================================================================
float_t PointLight::get_intensity(const float_t &distance) {
  // Intensity of a point light source distance away from a surface point
  // is governed by the Inverse square law.
  // I = intensity / (4 * PI * distance^2)
  auto denominator = static_cast<float_t>(4.f * M_PI * distance*distance);
  return i / denominator;
}
