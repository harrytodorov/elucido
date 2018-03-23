// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "Camera.h"
#include "../core/Common.h"


//==============================================================================
void Camera::rotate(const float_t &rot_angle, const Axis &rotation_axis) {
  apply_rotation(rotation_axis, rot_angle, vm);
}

//==============================================================================
void Camera::translate(const float_t &translation,
                       const Axis &translation_axis) {
  apply_translation(translation_axis, translation, vm);
}

//==============================================================================
void Camera::apply_inverse_view_transform(const std::vector<std::shared_ptr<Object>> &objects,
                                          const std::vector<std::shared_ptr<Light>> &lights) {
  glm::mat4 ivm = glm::inverse(vm);
  for (auto const &object : objects) {
    object->apply_camera_transformation(ivm);
  }
  for (auto const &light : lights) {
    light->apply_camera_transformation(ivm);
  }
}

//==============================================================================
void Camera::reverse_inverse_view_transform(const std::vector<std::shared_ptr<Object>> &objects,
                                            const std::vector<std::shared_ptr<Light>> &lights) {
  for (auto const &object : objects) {
    object->apply_camera_transformation(vm);
  }
  for (auto const &light : lights) {
    light->apply_camera_transformation(vm);
  }
}
