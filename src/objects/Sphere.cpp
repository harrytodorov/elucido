// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "Sphere.h"

//=============================================================================
bool Sphere::intersect(const Ray &r, isect_info &i) const {
  // variable to hold distance between ray's origin and intersection point(s)
  float_t t;

  // compute the vector l, between the sphere's center c and the ray's origin o
  glm::vec4 l = c - r.orig();

  // compute the projection of l, s, on the ray's direction (d)
  float_t s = glm::dot(l, r.dir());

  // compute the squared length of the vector l, l2
  float_t l2 = glm::dot(l, l);

  // if the l2 > r2 (origin of the ray is outside the sphere) and
  // s < 0, the projection of l on d is less than 0 (the ray's origin is behind
  // the sphere), we can reject that there is an intersection between the ray and
  // the sphere
  if (s < 0 && l2 > r2) return false;

  // compute the side m2, of a right triangle formed by s, l and m
  // using the Pythagorean theorem: m2 = l2 - s2
  float_t m2 = l2 - s * s;

  // if m2 > r2, the ray definitely misses the sphere
  if (m2 > r2) return false;

  // find the side, q, of the right triangle formed by r (sphere's radius),
  // m and q: q2 = r2 - m2; we want q, set_orig q = sqrt(r2 - m2)
  float_t q = sqrtf(r2 - m2);

  // find the smallest intersection point
  if (l2 > r2) t = s - q;
  else t = s + q;

  // tests passed; assign variables
  i.tn = t;
  i.ip = r.orig() + t * r.dir();
  i.ti = (uint32_t) -1;

  return true;
}

//=============================================================================
void Sphere::get_surface_properties(isect_info &i) const {
  // the sphere's normal is the normalized vector between the hit point and
  // the sphere's center
  i.ipn = glm::normalize(i.ip - c);
}

//=============================================================================
void Sphere::apply_camera_transformation(const glm::mat4 &ivm) {
  c = ivm * c;
  reshape_bb();
}

//=============================================================================
void Sphere::translate(const float_t &translation,
                       const Axis &axes_of_translation) {
  glm::vec3 tv = create_transformation_vector(axes_of_translation, translation);
  glm::mat4 tm = glm::translate(glm::mat4(1), tv);
  mt = tm * mt;
}

//=============================================================================
void Sphere::rotate(const float_t &angle_of_rotation,
                    const Axis &axes_of_rotation) {
  // Rotation does not effect implicit sphere.
}

//=============================================================================
void Sphere::scale(const float_t &scaling_factor,
                   const Axis &axes_of_scale) {
  if (axes_of_scale != XYZ) {
    std::cout << "Implicit sphere supports only uniform scaling (XYZ)!"
              << std::endl;
    return;
  }
  set_radius(r * scaling_factor);
}

//=============================================================================
void Sphere::set_radius(const float_t &r) {
  this->r = r;
  this->r2 = powf(r, 2.f);
  reshape_bb();
}

//=============================================================================
void Sphere::set_center(const glm::vec4 &p) {
  this->c = p;
  reshape_bb();
}

//=============================================================================
void Sphere::apply_transformations() {
  c = mt * c;
  reshape_bb();

  // Reset model transform matrix.
  mt = glm::mat4(1);
}
