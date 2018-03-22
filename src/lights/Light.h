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

#include "../core/Utilities.h"

class Light {
//==============================================================================
// Constructors & destructors
//==============================================================================
 public:
  Light() {}
  Light(const glm::vec3 &c, const float_t &i) :
      c(c),
      i(i)
  {}
  Light(const float_t &i) :
      i(i)
  {}

  virtual ~Light() = default;

//==============================================================================
// Function declarations
//==============================================================================
  inline glm::vec3 color() const { return this->c; }
  /**
   * Set the color (RGB) for a light source. Each component of the color
   * is clamped in range [0,1].
   * @param _c: The RGB vector containing the color.
   */
  inline void      set_color(const glm::vec3 &_c) {
    c = glm::clamp(_c, 0.f, 1.f);
  }
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

//==============================================================================
// Data members
//==============================================================================
 protected:
  glm::vec3 c{1.f};   // Color.
  float_t   i{1.f};   // Intensity.
  glm::mat4 mt{};     // Model transform matrix.
};

#endif //ELUCIDO_LIGHT_H
