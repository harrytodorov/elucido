// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_LIGHT_H
#define ELUCIDO_LIGHT_H

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>
#include <cstdio>

#include "Utilities.h"

class Light {

//=============================================================================
// Data members
//=============================================================================
 public:
  glm::vec3 color{white};
  float_t intensity{10.0};

//=============================================================================
// Constructors & destructors
//=============================================================================
// default light color is white
// default light intensity is 10
  Light() = default;

//=============================================================================
  Light(const glm::vec3 &c, const float_t &i) : color(c), intensity(i) {}

//=============================================================================
  explicit Light(const float_t &i) : intensity(i) {}

//=============================================================================
  virtual ~Light() = default;

//=============================================================================
// Function declarations
//=============================================================================
  virtual void apply_camera_transformation(const glm::mat4 &ictm,
                                           const glm::mat4 &itctm) = 0;
  virtual void apply_transformations() = 0;
  virtual void translate(const float_t &translation,
                         const uint32_t &axes_of_translation) = 0;
  virtual void rotate(const float_t &angle_of_rotation,
                      const uint32_t &axes_of_rotation) = 0;
  virtual void illuminate(const glm::vec4 &hit_point,
                          glm::vec4 &light_dir,
                          glm::vec3 &light_intensity,
                          float_t &distance) = 0;

 protected:
  glm::mat4 mt;   // model transform matrix of a light source
  // not all kind of light sources can be transformed
  // in some way, but to keep it clean, we define the matrix in
  // the abstract class
};

#endif //ELUCIDO_LIGHT_H
