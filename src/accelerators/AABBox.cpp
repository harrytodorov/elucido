// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "AABBox.h"
#include "../extra/Ray.h"

//==============================================================================
void AABBox::extend_by(const glm::vec4 &p) {
  // check min bound
  if (p.x < bounds[0].x) bounds[0].x = p.x;
  if (p.y < bounds[0].y) bounds[0].y = p.y;
  if (p.z < bounds[0].z) bounds[0].z = p.z;

  // check max bound
  if (p.x > bounds[1].x) bounds[1].x = p.x;
  if (p.y > bounds[1].y) bounds[1].y = p.y;
  if (p.z > bounds[1].z) bounds[1].z = p.z;
}

//==============================================================================
bool AABBox::intersect(const Ray &r) const {
  float_t tmin, tmax, tymin, tymax, tzmin, tzmax;

  tmin = (bounds[r.sign()[0]].x - r.orig().x) * r.inv_dir().x;
  tmax = (bounds[1 - r.sign()[0]].x - r.orig().x) * r.inv_dir().x;

  tymin = (bounds[r.sign()[1]].y - r.orig().y) * r.inv_dir().y;
  tymax = (bounds[1 - r.sign()[1]].y - r.orig().y) * r.inv_dir().y;

  if ((tmin > tymax) || (tymin > tmax))
    return false;

  if (tymin > tmin)
    tmin = tymin;

  if (tymax < tmax)
    tmax = tymax;

  tzmin = (bounds[r.sign()[2]].z - r.orig().z) * r.inv_dir().z;
  tzmax = (bounds[1 - r.sign()[2]].z - r.orig().z) * r.inv_dir().z;

  return !((tmin > tzmax) || (tzmin > tmax));
}

//==============================================================================
bool AABBox::intersect(const Ray &r, float_t &tBox) const {
  float_t tmin, tmax, tymin, tymax, tzmin, tzmax;

  tmin = (bounds[r.sign()[0]].x - r.orig().x) * r.inv_dir().x;
  tmax = (bounds[1 - r.sign()[0]].x - r.orig().x) * r.inv_dir().x;

  tymin = (bounds[r.sign()[1]].y - r.orig().y) * r.inv_dir().y;
  tymax = (bounds[1 - r.sign()[1]].y - r.orig().y) * r.inv_dir().y;

  if ((tmin > tymax) || (tymin > tmax))
    return false;

  if (tymin > tmin)
    tmin = tymin;

  if (tymax < tmax)
    tmax = tymax;

  tzmin = (bounds[r.sign()[2]].z - r.orig().z) * r.inv_dir().z;
  tzmax = (bounds[1 - r.sign()[2]].z - r.orig().z) * r.inv_dir().z;

  if ((tmin > tzmax) || (tzmin > tmax))
    return false;

  if (tzmin > tmin)
    tmin = tzmin;
  if (tzmax < tmax)
    tmax = tzmax;

  tBox = tmin;
  return true;
}