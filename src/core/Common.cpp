// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "Common.h"

//==============================================================================
glm::vec3 create_transformation_vector(const Axis &transformation_axes,
                                       const float_t &transformation_amount) {
  glm::vec3 tv(0);

  switch (transformation_axes) {
    case Axis::X :
      tv.x = transformation_amount;
      return tv;
    case Axis::Y :
      tv.y = transformation_amount;
      return tv;
    case Axis::Z :
      tv.z = transformation_amount;
      return tv;
    case Axis::uniform :
      tv = glm::vec3(transformation_amount);
      return tv;
    default:
      std::cout << "You're using an undefined axis of scale." << std::endl;
      return glm::vec3(1);
  }
}

//==============================================================================
void apply_rotation(const Axis &axis,
                    const float_t &rotation_angle,
                    glm::mat4 &model_transform) {
  // Rotations could be applied only on a single axis.
  if (axis == uniform) return;
  auto rv = create_transformation_vector(axis,
                                         1);
  auto rm = glm::rotate(glm::mat4(1), glm::radians(rotation_angle), rv);
  model_transform = rm * model_transform;
}

//==============================================================================
void apply_translation(const Axis &axis,
                       const float_t &translation_amount,
                       glm::mat4 &model_transform) {
  auto tv = create_transformation_vector(axis, translation_amount);
  auto tm = glm::translate(glm::mat4(1), tv);
  model_transform = tm * model_transform;
}

//==============================================================================
void apply_scale(const Axis &axis,
                 const float_t &scale_amount,
                 glm::mat4 &model_transform) {
  // Scale could be applied only uniformly.
  if (axis != uniform) return;
  glm::vec3 sv = create_transformation_vector(axis, scale_amount);
  glm::mat4 sm = glm::scale(glm::mat4(1), sv);
  model_transform = sm * model_transform;
}

//==============================================================================
bool triangle_intersect(const Ray &r,
                        const glm::vec4 &v0,
                        const glm::vec4 &v1,
                        const glm::vec4 &v2,
                        float_t &t,
                        float_t &u,
                        float_t &v,
                        bool &flip_normal) {
  auto edge1 = v1 - v0;
  auto edge2 = v2 - v0;

  auto p_vec = glm::vec4(glm::cross(glm::vec3(r.dir()), glm::vec3(edge2)), 0.f);
  auto determinant = glm::dot(edge1, p_vec);

  if (determinant > -kEpsilon && determinant < kEpsilon)
    return false;

  auto inv_determinant = 1.f / determinant;

  // Calculate distance vector from vertex 0 to the ray origin.
  auto t_vec = r.orig() - v0;

  // Calculate Barycentric u-parameter.
  u = static_cast<float_t>(glm::dot(t_vec, p_vec) * inv_determinant);

  // Test if u-parameter is in the bounds [0,1].
  if (u < 0.f || u > 1.f) return false;

  auto q_vec = glm::vec4(glm::cross(glm::vec3(t_vec), glm::vec3(edge1)), 0.f);

  // Calculate Barycentric v-parameter.
  v = static_cast<float_t>(glm::dot(r.dir(), q_vec) * inv_determinant);

  // Test if v-parameter is in the bounds [0,1].
  if (v < 0.f || v + u > 1.f) return false;

  // Calculate t.
  t = static_cast<float_t>(glm::dot(edge2, q_vec) * inv_determinant);

  // t should be positive.
  if (t < 0.f) return false;

  // If the determinant is less tha epsilon, normal direction
  // should be flipped.
  if (determinant < kEpsilon) flip_normal = true;

  return true;
}