// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_BBOX_H
#define ELUCIDO_BBOX_H

#include <glm/vec4.hpp>

#include "../core/Utilities.h"

class Ray;

class AABBox {
//==============================================================================
// Constructors & destructors
//==============================================================================
 public:
  AABBox() {}
  AABBox(const glm::vec4 &min, const glm::vec4 &max) {
    bounds[0] = min;
    bounds[1] = max;
  }

  ~AABBox() {}

//==============================================================================
// Function declarations, inline functions
//==============================================================================
  inline void reset() {
    bounds[0] = glm::vec4(glm::vec3(infinity), 1);
    bounds[1] = glm::vec4(glm::vec3(-infinity), 1);
  }
  inline float_t getVolume() const {
    glm::vec3 size(bounds[1] - bounds[0]);
    return glm::abs(size.x) * glm::abs(size.y) * glm::abs(size.z);
  }
  inline glm::vec4 getDiagonal() const {
    return glm::abs(glm::vec4(bounds[1] - bounds[0]));
  }
  bool intersect(const Ray &r) const;
  bool intersect(const Ray &r, float_t &t_min) const;
  void extend_by(const glm::vec4 &p);

//==============================================================================
// Data members
//==============================================================================
  glm::vec4 bounds[2] = {glm::vec4(glm::vec3(infinity), 1.f),
                         glm::vec4(glm::vec3(-infinity), 1.f)};
};

#endif //ELUCIDO_BBOX_H
