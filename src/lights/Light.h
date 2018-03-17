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

#include "../extra/Utilities.h"

class Light {
 public:
  Light() :
      c(glm::vec3(1, 1, 1)),
      i(1.f)
      {}
  Light(const glm::vec3 &c, const float_t &i) : c(c), i(i) {}
  Light(const float_t &i) : i(i) {}
  virtual ~Light() = default;

  inline glm::vec3 color() { return c; }
  inline void      set_color(const glm::vec3 &_c) { c = glm::normalize(_c); }
  inline void      set_intensity(const float_t &_i) { i = _i; }

  /**
   * Calculate the normalized direction vector pointing from a surface point
   * towards the light source.
   * @param surface_point:  The surface point away from which the direction
   *                        vector is pointing.
   * @return:               The normalized direction vector.
   */
  virtual glm::vec4 get_direction(const glm::vec4 &surface_point) = 0;

  /**
   * Calculate the distance between light source's position and a surface
   * point.
   * @param surface_point:  The surface point of interest.
   * @return:               The computed distance between the surface point
   *                        and the light source.
   */
  virtual float_t   get_distance(const glm::vec4 &surface_point) = 0;

  /**
   * Calculate the intensity which a surface point receives distance away from
   * the light source.
   * @param distance:   The distance of between the surface point and the
   *                    light source.
   * @return:           The computed intensity at the surface point.
   */
  virtual float_t get_intensity(const float_t &distance);

  virtual void translate(const float_t &translation,
                         const Axis &axes_of_translation) = 0;
  virtual void rotate(const float_t &angle_of_rotation,
                      const Axis &axes_of_rotation) = 0;
  virtual void apply_camera_transformation(const glm::mat4 &ivm) = 0;
  virtual void apply_transformations() = 0;

 protected:
  glm::mat4 mt;
  glm::vec3 c{white};
  float_t   i{10.0};
};

#endif //ELUCIDO_LIGHT_H
