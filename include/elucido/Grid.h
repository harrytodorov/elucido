// Copyright (c) 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_ALL_GRID_H
#define ELUCIDO_ALL_GRID_H

#include <vector>
#include "AccelerationStructure.h"
#include "Object.h"

class Grid : public AccelerationStructure {

//=============================================================================
// Container for a cell
//=============================================================================
  struct Cell {
    Cell() {}
    void insert(const Primitive &primitive) {
      primitives.emplace_back(primitive);
    }
    void intersect(const Ray &r, isect_info &i) {
      for (auto primitive : primitives) {
        primitive.intersect(r, i);
      }
    }
    std::vector<Primitive> primitives;
  };

 public:
  Grid(const AABBox &box,
       const std::vector<Object *> &objects);
  ~Grid() {
    for (size_t i = 0; i < resolution[0] * resolution[1] * resolution[2]; i++) {
      if (cells[i] != NULL) delete cells[i];
      delete [] cells;
    }
  }
  Cell **cells;
 private:
  size_t resolution[3];
  size_t maxResolution{128};
  float_t alpha{5.f};
  glm::vec4 cellDimension;
};

#endif //ELUCIDO_ALL_GRID_H
