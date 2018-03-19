// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "Triangle.h"

//==============================================================================
bool Triangle::intersect(const Ray &r, isect_info &i) const {

  auto edge1 = v1 - v0;
  auto edge2 = v2 - v0;

  auto p = glm::vec4(glm::cross(glm::vec3(r.dir()), glm::vec3(edge2)), 0.f);
  auto determinant = glm::dot(edge1, p);

  if (determinant > -kEpsilon && determinant < kEpsilon)
    return false;

  auto inv_determinant = 1.f / determinant;

  // Calculate distance vector from vertex 0 to the ray origin.
  auto d = r.orig() - v0;

  // Calculate Barycentric u-parameter.
  auto u = glm::dot(d, p) * inv_determinant;

  // Test if u-parameter is in the bounds [0,1].
  if (u < 0.f || u > 1.f) return false;

  auto q = glm::vec4(glm::cross(glm::vec3(d), glm::vec3(edge1)), 0.f);

  // Calculate Barycentric v-parameter.
  auto v = glm::dot(r.dir(), q) * inv_determinant;

  // Test if v-parameter is in the bounds [0,1].
  if (v < 0.f || v > 1.f) return false;

  // Calculate t.
  auto t = glm::dot(edge2, q) * inv_determinant;

  i.tn  = static_cast<float_t>(t);
  i.ip  = r.orig() + t*r.dir();
  i.ipn = n;
  i.u   = static_cast<float_t>(u);
  i.v   = static_cast<float_t>(v);

  return true;
}

//==============================================================================
bool Triangle::shadow_intersect(const Ray &r) const {
  auto edge1 = v1 - v0;
  auto edge2 = v2 - v0;

  auto p = glm::vec4(glm::cross(glm::vec3(r.dir()), glm::vec3(edge2)), 0.f);
  auto determinant = glm::dot(edge1, p);

  if (determinant > -kEpsilon && determinant < kEpsilon)
    return false;

  auto inv_determinant = 1.f / determinant;

  // Calculate distance vector from vertex 0 to the ray origin.
  auto d = r.orig() - v0;

  // Calculate Barycentric u-parameter.
  auto u = glm::dot(d, p) * inv_determinant;

  // Test if u-parameter is in the bounds [0,1].
  if (u < 0.f || u > 1.f) return false;

  auto q = glm::vec4(glm::cross(glm::vec3(d), glm::vec3(edge1)), 0.f);

  // Calculate Barycentric v-parameter.
  auto v = glm::dot(r.dir(), q) * inv_determinant;

  // Test if v-parameter is in the bounds [0,1].
  if (v < 0.f || v > 1.f) return false;

  return true;
}

//==============================================================================
void Triangle::get_surface_properties(isect_info &i) const {
  i.ipn = n;
}

//==============================================================================
void Triangle::apply_camera_transformation(const glm::mat4 &ivm) {
  v0 = ivm * v0;
  v1 = ivm * v1;
  v2 = ivm * v2;
  reshape_bb();

  n = glm::transpose(glm::inverse(ivm)) * n;
  // Reset normal's w component to 0.
  n.w = 0.f;
  // Renormalize.
  n = glm::normalize(n);
}

//==============================================================================
void Triangle::apply_transformations() {
  v0 = mt * v0;
  v1 = mt * v1;
  v2 = mt * v2;
  reshape_bb();

  n = glm::transpose(glm::inverse(mt)) * n;
  // Reset normal's w component to 0.
  n.w = 0.f;
  // Renormalize.
  n = glm::normalize(n);

  // Reset model transform matrix.
  mt = glm::mat4(1);
}

//==============================================================================
void Triangle::calculate_normal() {
  auto a = glm::vec3(v1.x - v0.x,
                     v1.y - v0.y,
                     v1.z - v0.z);
  auto b = glm::vec3(v2.x - v0.x,
                     v2.y - v0.y,
                     v2.z - v0.z);
  auto c = glm::cross(a, b);
  n = glm::normalize(glm::vec4(c.x, c.y, c.z, 0.f));
}

//==============================================================================
void Triangle::reshape_bb() {
  // we don't want the bounding box to maintain its old shape
  bb.reset();

  // extend bb of triangle
  bb.extend_by(v0);
  bb.extend_by(v1);
  bb.extend_by(v2);
}