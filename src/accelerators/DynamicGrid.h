// Copyright (c) 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_ALL_GRID_H
#define ELUCIDO_ALL_GRID_H

#include <vector>
#include <memory>

#include "AccelerationStructure.h"
#include "Grid.h"

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

class DynamicGrid : public AccelerationStructure, public Grid {
//==============================================================================
// Constructors & destructors
//==============================================================================
 public:
  DynamicGrid() :
      AccelerationStructure(),
      Grid()
  {
    as_type = grid;
  }

  ~DynamicGrid() {}

//==============================================================================
// Function declarations
//==============================================================================
  void            construct(const AABBox &box,
                            const std::vector<std::shared_ptr<Object>> &objects,
                            const uint32_t &number_primitives,
                            as_construct_info &info);
  bool            traverse(const Ray &r, isect_info &i) const;

//==============================================================================
// Data members
//==============================================================================
 private:
  std::vector<std::shared_ptr<Cell>>    cells{};
};

#endif //ELUCIDO_ALL_GRID_H
