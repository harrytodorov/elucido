// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_POINTLIGHT_H
#define ELUCIDO_POINTLIGHT_H

#include "Light.h"

class PointLight : public Light {
 public:
  PointLight() = default;
  PointLight(const glm::vec4 &p, const float_t &i) :
      Light(i),
      p(p)
  {}
  PointLight(const glm::vec4 &p, const glm::vec3 &c, const float_t &i) :
      Light(c, i),
      p(p)
  {}
  ~PointLight() = default;

  glm::vec4 get_direction(const glm::vec4 &surface_point);
  float_t   get_distance(const glm::vec4 &surface_point);
  float_t   get_intensity(const float_t &distance);

  inline glm::vec4 position() const { return p; }
  inline void      set_position(const glm::vec4 &_p) { p = _p; }

  void apply_camera_transformation(const glm::mat4 &ivm);
  void apply_transformations();
  void translate(const float_t &translation,
                 const Axis &translation_axis);
  void rotate(const float_t &angle_of_rotation,
              const Axis &axes_of_rotation);

 protected:
  glm::vec4 p{0.f, 0.f, 0.f, 1.f};  // Position.
};

#endif //ELUCIDO_POINTLIGHT_H