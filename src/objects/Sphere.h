// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_SPHERE_H
#define ELUCIDO_SPHERE_H

#include "Object.h"

class Sphere : public Object {
 public:
  Sphere() :
      Object() {
    r2 = r*r;
    reshape_bb();
    ot = sphere;
  }
  Sphere(const glm::vec4 &c, const float_t &r) :
      Object(),
      r(r),
      c(c) {
    r2 = powf(r, 2.f);
    reshape_bb();
    ot = sphere;
  }
  Sphere(const glm::vec4 &c, const float_t &r, const struct material &m) :
      Object(m),
      r(r),
      c(c) {
    r2 = glm::pow(r, 2.f);
    reshape_bb();
    ot = sphere;
  }
  Sphere(const Sphere &s) :
      Object(s) {
    this->r = s.r;
    this->c = s.c;
    this->r2 = glm::pow(r, 2.f);
    ot = sphere;
  }
  ~Sphere() {}

  inline float_t& radius() { return this->r; }
  void            set_radius(const float_t &r);

  inline glm::vec4 center() { return this->c; }
  void             set_center(const glm::vec4 &p);

  bool intersect(const Ray &r, isect_info &i) const;
  virtual void get_surface_properties(isect_info &i) const;

  void apply_camera_transformation(const glm::mat4 &ivm);
  void apply_transformations();
  void rotate(const float_t &angle_of_rotation,
              const Axis &axes_of_rotation);
  void scale(const float_t &scaling_factor, const Axis &axes_of_scale);

 private:
  inline void reshape_bb() {
    bb.reset();
    bb.extend_by(glm::vec4(c.x - r, c.y - r, c.z - r, 1));
    bb.extend_by(glm::vec4(c.x + r, c.y + r, c.z + r, 1));
  }

 protected:
  float_t   r{1.f};                     // Radius.
  float_t   r2{};                       // Radius^2.
  glm::vec4 c{0.f, 0.f, 0.f, 1.f};      // Center position.
};

#endif //ELUCIDO_SPHERE_H
