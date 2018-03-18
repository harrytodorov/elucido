// Copyright (c) 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "AccelerationStructure.h"

std::vector<Primitive> AccelerationStructure::convertToPrimitive(
    const std::shared_ptr<Object> &obj) {
  std::vector<Primitive> result;

  // If the object is not a triangle mesh, it's already a primitive.
  if (obj->object_type() != triangle_mesh) {
    result.emplace_back(obj, 0);
    return result;
  }

  // The object is a triangle mesh. Iterate over its triangles and pack
  // them in a vector of primitives.
  auto mesh = std::static_pointer_cast<TriangleMesh>(obj);
  for (size_t i = 0; i < mesh->nt; i++) {
    result.emplace_back(obj, i);
  }
  return result;
}
