// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_RAY_H
#define ELUCIDO_RAY_H

#include "Utilities.h"

class Ray {
//==============================================================================
// Constructors & destructors
//==============================================================================
 public:
  RayType rt{primary};  // the type of the casted ray

  Ray() {}
  ~Ray() = default;

//==============================================================================
// Function declarations
//==============================================================================
  inline glm::vec4 dir()     const { return this->d; }
  inline glm::vec4 inv_dir() const { return this->id; }
  void             set_dir(const glm::vec4 &_d);

  inline glm::vec4 orig() const { return this->o; }
  inline void      set_orig(const glm::vec4 &_o) { this->o = _o; }

  inline const uint32_t *sign() const { return this->s; }

//==============================================================================
// Data members
//==============================================================================
 private:
  glm::vec4 o{0.f};                   // the origin point of the ray
  glm::vec4 d{0.f,  0.f, -1.f, 0.f};  // the direction of the ray
  glm::vec4 id{0.f, 0.f,  1.f, 0.f};; // the inverse of the ray direction,
  uint32_t s[3]{};                    // the sign of the ray direction,
};

#endif //ELUCIDO_RAY_H
