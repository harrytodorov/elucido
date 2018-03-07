// Copyright (c) 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_ACCELERATIONSTRUCTURE_H
#define ELUCIDO_ACCELERATIONSTRUCTURE_H

#include "../objects/Object.h"
#include "../objects/TriangleMesh.h"

struct Primitive {
  Primitive() {}
  Primitive(const Object &obj, const size_t ti) {
    obj_pointer = &obj;
    tri_ind = ti;
  }
  bool intersect(const Ray &r, isect_info &ii) {
    if (obj_pointer->ot == triangle_mesh) {
      return static_cast<const TriangleMesh*>(obj_pointer)->intersect(r,
                                                                      tri_ind,
                                                                      ii);
    }
    return obj_pointer->intersect(r, ii);
  }
  const AABBox* getBB() {
    if (obj_pointer->ot == triangle_mesh) {
      return static_cast<const TriangleMesh*>(obj_pointer)->
          getBoundingBoxForTriangle(tri_ind);
    }
    return &(obj_pointer->bb);
  }

  const Object *obj_pointer;
  size_t tri_ind;
};

class AccelerationStructure {
 public:
  AccelerationStructure(const AABBox &box,
                        const std::vector<Object *> &objects) :
  bbox(box) {
    for (auto object : objects) {
      // TODO: Precalculate number of primitive during scene construction.
      std::vector<Primitive> toPrimitive = convertToPrimitive(*object);
      // Reserve space.
      primitives.reserve(primitives.size() + toPrimitive.size());
      primitives.insert(primitives.end(),
                        toPrimitive.begin(),
                        toPrimitive.end());
    }
  }

  std::vector<Primitive> convertToPrimitive(const Object &obj);
  virtual bool intersect(const Ray &r, isect_info &i) const = 0;
  AABBox bbox;
  std::vector<Primitive> primitives;
};

#endif //ELUCIDO_ACCELERATIONSTRUCTURE_H
