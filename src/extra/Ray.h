// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_RAY_H
#define ELUCIDO_RAY_H

#include "glm/vec4.hpp"

#include <vector>

#include "../objects/Object.h"
#include "Utilities.h"

class Ray {
 public:
  RayType rt{primary};  // the type of the casted ray

  Ray() {}
  ~Ray() = default;

  bool trace(const std::vector<Object *> &objects,
             isect_info &ii,
             render_info &ri) const;
  void set_dir(const glm::vec4 &_d);
  inline glm::vec4 orig() const { return this->o; }
  inline glm::vec4 dir() const { return this->d; }
  inline const uint32_t *sign() const { return this->s; }
  inline glm::vec4 inv_dir() const { return this->id; }
  inline void set_orig(const glm::vec4 &_o) { this->o = _o; }

 protected:
  glm::vec4 o;          // the origin point of the ray
  glm::vec4 d;          // the direction of the ray
  glm::vec4 id;         // the inverse of the ray direction,
  // needed to optimize AABB calculation
  uint32_t s[3]{};      // the sign of the ray direction,
  // needed to optimize AABB calculation
};

#endif //ELUCIDO_RAY_H
