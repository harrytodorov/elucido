// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_POINTLIGHT_H
#define ELUCIDO_POINTLIGHT_H

#include "Light.h"

class PointLight : public Light {

//=============================================================================
// Data members
//=============================================================================
 public:
  glm::vec4 p{glm::vec4(0, 0, 0, 1)};

//=============================================================================
// Constructors & destructors
//=============================================================================
  PointLight() = default;

//=============================================================================
  PointLight(const glm::vec4 &p, const float_t &i) : Light(i), p(p) {}

//=============================================================================
  PointLight(const glm::vec4 &p, const glm::vec3 &c, const float_t &i)
      : Light(c, i), p(p) {}

//=============================================================================
  ~PointLight() = default;

//=============================================================================
// Function declarations
//=============================================================================
  void illuminate(const glm::vec4 &hit_point,
                  glm::vec4 &light_dir,
                  glm::vec3 &light_intensity,
                  float_t &distance);
  void apply_camera_transformation(const glm::mat4 &ivm);
  void apply_transformations();
  void translate(const float_t &translation,
                 const Axis &translation_axis);
  void rotate(const float_t &angle_of_rotation,
              const Axis &axes_of_rotation);
};

#endif //ELUCIDO_POINTLIGHT_H