// Copyright (c) 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "Grid.h"

Grid::Grid(const AABBox &box, const std::vector<const Object> &_objects)  :
    AccelerationStructure(box, _objects), cells(NULL) {

  // compute number of primitives in the scene
  size_t numPrimitives{0};
  for (auto &object : _objects) {
    if (object.ot == triangle_mesh) {
      numPrimitives += static_cast<const TriangleMesh*>(&object)->nt;
    } else {
      numPrimitives++;
    }
  }

}
