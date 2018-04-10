// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "Triangle.h"
#include "../core/Common.h"

//==============================================================================
bool Triangle::intersect(const Ray &r, isect_info &i) const {
  float_t t, u, v;
  bool fp{false};

  auto intersected = triangle_intersect(r, v0, v1, v2, t, u, v, fp);
  if (!intersected) return false;

  i.tn  = static_cast<float_t>(t);
  i.ip  = r.orig() + t*r.dir();
  i.ipn = (fp) ? -n : n;
  i.u   = static_cast<float_t>(u);
  i.v   = static_cast<float_t>(v);

  return true;
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

//==============================================================================
glm::vec4 Triangle::centroid(const uint32_t &ti) const {
  glm::vec4 centroid = (v0 + v1 + v2) / 3.f;
  return centroid;
}
