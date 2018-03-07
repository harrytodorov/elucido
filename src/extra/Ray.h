// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_RAY_H
#define ELUCIDO_RAY_H

#include "glm/vec4.hpp"

#include <vector>

#include "../objects/Object.h"
#include "Utilities.h"

class Ray {

//=============================================================================
// Data members
//=============================================================================
  glm::vec4 o;          // the origin point of the ray
  glm::vec4 d;          // the direction of the ray
  glm::vec4 id;         // the inverse of the ray direction,
                        // needed to optimize AABB calculation
  uint32_t s[3];        // the sign of the ray direction,
                        // needed to optimize AABB calculation
 public:
  RayType rt{primary};  // the type of the casted ray

//=============================================================================
// Constructors & destructors
//=============================================================================
  Ray() : o(0, 0, 0, 1), d(0, 0, -1, 0) {
    id = 1.f / d;
    s[0] = (uint32_t) (id.x < 0);
    s[1] = (uint32_t) (id.y < 0);
    s[2] = (uint32_t) (id.z < 0);
  }

//=============================================================================
  Ray(const glm::vec4 &o, const glm::vec4 &d) : o(o), d(d) {
    id = 1.f / d;
    s[0] = (uint32_t) (id.x < 0);
    s[1] = (uint32_t) (id.y < 0);
    s[2] = (uint32_t) (id.z < 0);
  }

//=============================================================================
  ~Ray() = default;

//=============================================================================
// Function declarations, inline functions
//=============================================================================
  bool trace(const std::vector<Object *> &objects,
             isect_info &ii,
             render_info &ri) const;
  void set_dir(const glm::vec4 &_d);
  inline glm::vec4 orig() const { return this->o; }
  inline glm::vec4 dir() const { return this->d; }
  inline const uint32_t *sign() const { return this->s; }
  inline glm::vec4 inv_dir() const { return this->id; }
  inline void set_orig(const glm::vec4 &_o) { this->o = _o; }
};

#endif //ELUCIDO_RAY_H
