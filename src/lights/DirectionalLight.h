// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_DIRECTIONALLIGHT_H
#define ELUCIDO_DIRECTIONALLIGHT_H

#include "Light.h"

class DirectionalLight : public Light {
//==============================================================================
// Constructors & destructors
//==============================================================================
 public:
  DirectionalLight() = default;

  ~DirectionalLight() = default;

//==============================================================================
// Function declarations
//==============================================================================
  glm::vec4 get_direction(const glm::vec4 &surface_point);
  float_t   get_distance(const glm::vec4 &surface_point);

  inline glm::vec4 direction() { return d; }
  inline void      set_direction(const glm::vec4 &_d) { d = glm::normalize(_d); }

  void apply_camera_transformation(const glm::mat4 &ivm);
  void apply_transformations();
  void translate(const float_t &translation,
                 const Axis &axes_of_translation);
  void rotate(const float_t &angle_of_rotation,
              const Axis &rotation_axis);

//==============================================================================
// Data members
//==============================================================================
 protected:
  glm::vec4 d{0.f};  // Direction.
};

#endif //ELUCIDO_DIRECTIONALLIGHT_H
