// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "Sphere.h"

//==============================================================================
bool Sphere::intersect(const Ray &r, isect_info &i) const {
  float_t t;

  // Compute the distance vector between the
  // sphere's center and the ray's origin.
  glm::vec4 l = c - r.orig();

  // Compute the projection of l onto the ray's direction.
  float_t s = glm::dot(l, r.dir());

  // Compute the |l|^2.
  float_t l2 = glm::dot(l, l);

  // If the l2 > r2 (origin of the ray is outside the sphere) and
  // s < 0 (the ray's origin is behind the sphere), one can reject
  // that there is an intersection between the ray and the sphere.
  if (s < 0 && l2 > r2) return false;

  // Compute m^2, a side of a right triangle formed by s, l and m
  // using the Pythagorean theorem:
  // m^2 = l^2 - s^2
  float_t m2 = l2 - s * s;

  // If m^2 > r^2, ray misses the sphere.
  if (m2 > r2) return false;

  // Find the side, q, of a right triangle formed by r,m and q:
  // q^2 = r^2 - m^2
  float_t q = sqrtf(r2 - m2);

  // Find the smallest intersection point
  if (l2 > r2) t = s - q;
  else t = s + q;

  i.tn = t;
  i.ip = r.orig() + t * r.dir();
  i.ti = static_cast<uint32_t>(-1);

  return true;
}

//==============================================================================
void Sphere::get_surface_properties(isect_info &i) const {
  // the sphere's normal is the normalized vector between the hit point and
  // the sphere's center
  i.ipn = glm::normalize(i.ip - c);
}

//==============================================================================
void Sphere::apply_camera_transformation(const glm::mat4 &ivm) {
  c = ivm * c;
  reshape_bb();
}

//==============================================================================
void Sphere::rotate(const float_t &angle_of_rotation,
                    const Axis &axes_of_rotation) {
  // Rotation does not effect implicit sphere.
}

//==============================================================================
void Sphere::scale(const float_t &scaling_factor,
                   const Axis &axes_of_scale) {
  if (axes_of_scale != uniform) {
    std::cout << "Implicit sphere supports only uniform scaling (uniform)!"
              << std::endl;
    return;
  }
  set_radius(r * scaling_factor);
}

//==============================================================================
void Sphere::set_radius(const float_t &r) {
  this->r = r;
  this->r2 = powf(r, 2.f);
  reshape_bb();
}

//==============================================================================
void Sphere::set_center(const glm::vec4 &p) {
  this->c = p;
  reshape_bb();
}

//==============================================================================
void Sphere::apply_transformations() {
  c = mt * c;
  reshape_bb();

  // Reset model transform matrix.
  mt = glm::mat4(1);
}
