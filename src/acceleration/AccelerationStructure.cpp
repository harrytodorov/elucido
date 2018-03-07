// Copyright (c) 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "AccelerationStructure.h"

std::vector<Primitive> AccelerationStructure::convertToPrimitive(
    const Object &obj) {
  std::vector<Primitive> result;

  // If the object is not a triangle mesh, it's already a primitive.
  if (obj.ot != triangle_mesh) {
    result.emplace_back(obj, 0);
    return result;
  }

  // The object is a triangle mesh. Iterate over its triangles and pack
  // them in a vector of primitives.
  TriangleMesh mesh = dynamic_cast<const TriangleMesh&>(obj);
  for (size_t i = 0; i < mesh.nt; i++) {
    result.emplace_back(obj, i);
  }
  return result;
}
