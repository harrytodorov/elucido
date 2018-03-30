// Copyright (c) 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_ALL_GRID_H
#define ELUCIDO_ALL_GRID_H

#include <vector>
#include <memory>

#include "AccelerationStructure.h"
#include "../objects/Object.h"

struct Cell {
//==============================================================================
// Constructors & destructors
//==============================================================================
  Cell() {}
  ~Cell() {}

//==============================================================================
// Function declarations
//==============================================================================
  inline void insert(const Primitive &primitive) {
    primitives.emplace_back(primitive);
  }

  inline bool intersect(const Ray &r, isect_info &i) const {
    for (auto const &primitive : primitives) {
      // Information we got from the intersection with the current object.
      isect_info co;

      if (primitive.intersect(r, co) && co.tn < i.tn) {
        i = co;
        i.ho = primitive.obj;
        if (i.ho->object_type() == triangle_mesh) {
          i.ti = primitive.tri_ind;
        }
      }
    }

    return (i.ho != nullptr);
  }

//==============================================================================
// Data members
//==============================================================================
  std::vector<Primitive> primitives;
};

class Grid : public AccelerationStructure {
//==============================================================================
// Constructors & destructors
//==============================================================================
 public:
  Grid() :
      AccelerationStructure()
  {
    as_type = grid;
  }

  ~Grid() {}

//==============================================================================
// Function declarations
//==============================================================================
  void            construct(const AABBox &box,
                            const std::vector<std::shared_ptr<Object>> &objects,
                            const uint32_t &number_primitives,
                            as_construct_info &info);
  bool            traverse(const Ray &r, isect_info &i) const;

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

//==============================================================================
// Data members
//==============================================================================
 private:
  std::vector<std::shared_ptr<Cell>>    cells{};
  uint32_t                              resolution[3];
  uint32_t                              maxResolution{64};
  float_t                               alpha{3.f};
  glm::vec4                             cellDimension{0.f};
};

#endif //ELUCIDO_ALL_GRID_H
