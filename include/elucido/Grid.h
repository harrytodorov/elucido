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
    void insert(const Object *object) {
      primitives.push_back(Primitive(object, 0));
    }
    void insert(const Object *object, const uint32_t &tri_ind) {
      primitives.push_back(Primitive(object, tri_ind));
    }
    void intersect(const Ray &r, isect_info &i) {
      for (auto primitive : primitives) {
        primitive.intersect(r, i);
      }
    }
    std::vector<Primitive> primitives;
  };

 public:
  Grid(const AABBox &box, const std::vector<const Object> &_objects);
  ~Grid() {
    for (size_t i = 0; i < resolution[0] * resolution[1] * resolution[2]; i++) {
      if (cells[i] != NULL) delete cells[i];
      delete [] cells;
    }
  }

  Cell **cells;
  size_t resolution[3];
  glm::vec3 cellDimention;
};

#endif //ELUCIDO_ALL_GRID_H
