// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_SPHERE_H
#define ELUCIDO_SPHERE_H

#include "Object.h"

class Sphere : public Object {
//==============================================================================
// Data members
//==============================================================================
  float_t r;
  float_t r2;
  glm::vec4 c;

 public:
//==============================================================================
// Constructors & destructors
//==============================================================================
  Sphere() : Object(), r(1.0), c(0, 0, 0, 1) {
    r2 = 1.f;
    reshape_bb();
    ot = sphere;
  }

//==============================================================================
  Sphere(const material &m) : Object(m), r(1.0), c(0, 0, 0, 1) {
    r2 = 1.f;
    reshape_bb();
    ot = sphere;
  }

//==============================================================================
  Sphere(const glm::vec4 &c, const float_t &r) : Object(), r(r), c(c) {
    r2 = powf(r, 2.f);
    reshape_bb();
    ot = sphere;
  }

//==============================================================================
  Sphere(const glm::vec4 &c, const float_t &r, const material &m) : Object(m),
                                                                    r(r),
                                                                    c(c) {
    r2 = glm::pow(r, 2.f);
    reshape_bb();
    ot = sphere;
  }

//==============================================================================
  Sphere(const Sphere &s) : Object(s) {
    this->r = s.r;
    this->c = s.c;
    this->r2 = glm::pow(r, 2.f);
    ot = sphere;
  }

//==============================================================================
  ~Sphere() {}


//==============================================================================
// Function declarations
//==============================================================================
  void set_radius(const float_t &r);
  void set_center(const glm::vec4 &p);
  inline glm::vec4 center() { return this->c; }
  inline float_t radius() { return this->r; }
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
};

#endif //ELUCIDO_SPHERE_H
