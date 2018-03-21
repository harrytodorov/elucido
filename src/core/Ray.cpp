// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "Ray.h"

//==============================================================================
void Ray::set_dir(const glm::vec4 &_d) {
  this->d = _d;
  id = 1.f / d;
  s[0] = (uint32_t) (id.x < 0);
  s[1] = (uint32_t) (id.y < 0);
  s[2] = (uint32_t) (id.z < 0);
}
