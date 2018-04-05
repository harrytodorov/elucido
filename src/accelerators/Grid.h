// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_GRID_H
#define ELUCIDO_GRID_H

#include <cstdint>
#include <cmath>
#include <glm/vec4.hpp>


#include "AABBox.h"
#include "../core/Ray.h"

class Grid {
//==============================================================================
// Constructors & destructors
//==============================================================================
 public:
  Grid() {}
  ~Grid() = default;

  //==============================================================================
// Function declarations
//==============================================================================
  inline uint32_t offset(const uint32_t &x,
                         const uint32_t &y,
                         const uint32_t &z) const {
    return z * resolution[0] * resolution[1] + y * resolution[0] + x;
  }

  inline float_t  getAlpha() const { return this->alpha; }
  inline void     set_alpha(const float_t _alhpa) { this->alpha = _alhpa; }

  inline void     set_max_res(const uint32_t &resolution) {
    this->maxResolution = resolution;
  }

  void compute_resolution(const AABBox &box, const uint32_t &number_primitives);
  uint32_t* compute_primitive_bound_cell(const glm::vec4 & primitive_bound,
                                         const glm::vec4 & box_bound) const;
  void traversal_initialization(glm::vec4 &delta_t,
                                glm::vec4 &next_crossing_t,
                                uint32_t *current_cell,
                                int32_t *step,
                                int32_t *exit,
                                const Ray &ray,
                                const float_t &t_bb,
                                const glm::vec4 &box_min_bound) const;

//==============================================================================
// Data members
//==============================================================================
 protected:
  uint32_t                              resolution[3]{};
  uint32_t                              maxResolution{64};
  float_t                               alpha{3.f};
  glm::vec4                             cellDimension{0.f};
};

#endif //ELUCIDO_GRID_H
