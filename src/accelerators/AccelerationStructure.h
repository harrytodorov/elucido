// Copyright (c) 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_ACCELERATIONSTRUCTURE_H
#define ELUCIDO_ACCELERATIONSTRUCTURE_H

#include "../objects/Object.h"
#include "../objects/TriangleMesh.h"

struct Primitive {
  Primitive() {}
  Primitive(const std::shared_ptr<Object> &_obj, const uint32_t &ti) {
    obj = _obj;
    tri_ind = ti;
  }
  bool intersect(const Ray &r, isect_info &ii) {
    if (obj->object_type() == triangle_mesh) {
      return std::static_pointer_cast<TriangleMesh>(obj)->intersect_triangle(r,
                                                                             tri_ind,
                                                                             ii);
    }
    return obj->intersect(r, ii);
  }
  const AABBox* getBB() {
    if (obj->object_type() == triangle_mesh) {
      return std::static_pointer_cast<TriangleMesh>(obj)->get_BB(tri_ind);
    }
    return &(obj->bounding_box());
  }

  std::shared_ptr<Object> obj;
  uint32_t                tri_ind;
};

class AccelerationStructure {
 public:
  AccelerationStructure(const AABBox &box,
                        const std::vector<std::shared_ptr<Object>> &objects) :
  bbox(box) {
    for (auto const &object : objects) {
      // TODO: Precalculate number of primitive during scene construction.
      std::vector<Primitive> toPrimitive = convertToPrimitive(object);
      // Reserve space.
      primitives.reserve(primitives.size() + toPrimitive.size());
      primitives.insert(primitives.end(),
                        toPrimitive.begin(),
                        toPrimitive.end());
    }
  }

  std::vector<Primitive> convertToPrimitive(const std::shared_ptr<Object> &obj);
  virtual bool intersect(const Ray &r, isect_info &i) const = 0;
  AABBox bbox;
  std::vector<Primitive> primitives;
};

#endif //ELUCIDO_ACCELERATIONSTRUCTURE_H
