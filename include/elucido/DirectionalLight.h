// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_DIRECTIONALLIGHT_H
#define ELUCIDO_DIRECTIONALLIGHT_H

#include "Light.h"

class DirectionalLight : public Light {

//=============================================================================
// Data members
//=============================================================================
 public:
  glm::vec4 d{glm::vec4(0, 0, -1, 0)};

//=============================================================================
// Constructors & destructors
//=============================================================================
  DirectionalLight() = default;

//=============================================================================
  DirectionalLight(const glm::vec4 &d, const float_t &i)
      : Light(i), d(glm::normalize(d)) {}

//=============================================================================
  DirectionalLight(const glm::vec4 &d, const glm::vec3 &c, const float_t &i)
      : Light(c, i), d(glm::normalize(d)) {}

//=============================================================================
  ~DirectionalLight() = default;


//=============================================================================
// Function declarations
//=============================================================================
  void illuminate(const glm::vec4 &hit_point,
                  glm::vec4 &light_dir,
                  glm::vec3 &light_intensity,
                  float_t &distance);
  void apply_camera_transformation(const glm::mat4 &ictm,
                                   const glm::mat4 &itctm);
  void apply_transformations();
  void translate(const float_t &translation,
                 const Axis &axes_of_translation);
  void rotate(const float_t &angle_of_rotation,
              const Axis &axes_of_rotation);
};

#endif //ELUCIDO_DIRECTIONALLIGHT_H
