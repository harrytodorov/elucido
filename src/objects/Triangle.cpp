// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "Triangle.h"

//==============================================================================
bool Triangle::intersect(const Ray &r, isect_info &i) const {

  // check if ray is parallel to triangle; compute the dot product
  // of the triangle's normal and the ray direction
  // if ray and triangle's normal are close to 0, they don't intersect
  float_t nomal_ray_d_prod = glm::dot(n, r.dir());
  if (fabs(nomal_ray_d_prod) < kEpsilon) return false;

  // compute the dot product of the vector between one of the triangle's vertices and
  // the ray's origin and the triangle's normal
  float_t t_nominator = glm::dot(v0 - r.orig(), n);

  // compute the distance between the ray's origin and the hit point
  // with the triangle
  float_t t = t_nominator / nomal_ray_d_prod;

  // if the computed distance is negative, the triangle is behind the ray's origin
  if (t < kEpsilon) return false;

  // compute the possible intersection point
  glm::vec4 ip = r.orig() + t * r.dir();

  // check if the point is within the defined triangle
  glm::vec4 perp_vec;
  float_t d_prod;

  // edge 0
  glm::vec4 edge0(v1 - v0);
  glm::vec4 vp0(ip - v0);
  perp_vec = glm::vec4(glm::cross(glm::vec3(edge0), glm::vec3(vp0)), 0);
  d_prod = glm::dot(n, perp_vec);
  if (d_prod < 0) return false;

  // edge 1
  glm::vec4 edge1(v2 - v1);
  glm::vec4 vp1(ip - v1);
  perp_vec = glm::vec4(glm::cross(glm::vec3(edge1), glm::vec3(vp1)), 0);
  d_prod = glm::dot(n, perp_vec);
  if (d_prod < 0) return false;

  // edge 2
  glm::vec4 edge2(v0 - v2);
  glm::vec4 vp2(ip - v2);
  perp_vec = glm::vec4(glm::cross(glm::vec3(edge2), glm::vec3(vp2)), 0);
  d_prod = glm::dot(n, perp_vec);
  if (d_prod < 0) return false;

  // tests passed; assign variables
  i.tn = t;
  i.ip = ip;
  i.ti = (uint32_t) -1;

  return true;
}

//==============================================================================
bool Triangle::shadow_intersect(const Ray &r) const {
  return false;
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
  auto c = glm::cross(glm::vec3(v1) - glm::vec3(v0),
                      glm::vec3(v2) - glm::vec3(v0));
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