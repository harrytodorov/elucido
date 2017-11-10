// Copyright (c) 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_ACCELERATIONSTRUCTURE_H
#define ELUCIDO_ACCELERATIONSTRUCTURE_H

#include "Object.h"
#include "TriangleMesh.h"

class AccelerationStructure {
 protected:
//=============================================================================
// Container for a primitive
//=============================================================================
// Define a container to hold a primitive object
// Consist of a pair: pointer to an object and triangle index
// in case the object pointed to is a Triangle mesh
  struct Primitive {
    Primitive(const Object *obj, const uint32_t ti) {
      obj_pointer = obj;
      tri_ind = ti;
    }
    bool intersect(const Ray &r, isect_info &i) {
      if (obj_pointer->ot == triangle_mesh) {
        return static_cast<const TriangleMesh*>(obj_pointer)->intersect(r,
                                                                        tri_ind,
                                                                        i);
      }
      return obj_pointer->intersect(r, i);
    }
    const AABBox* getBoundingBox() {
      if (obj_pointer->ot == triangle_mesh) {
        return static_cast<const TriangleMesh*>(obj_pointer)->
            getBoundingBoxForTriangle(tri_ind);
      }
      return &(obj_pointer->bb);
    }

    const Object *obj_pointer;
    uint32_t tri_ind;
  };

 public:
  AccelerationStructure(const AABBox &box,
                        const std::vector<const Object> &_objects)
      : bbox(box) {}
  const AABBox bbox;
};

#endif //ELUCIDO_ACCELERATIONSTRUCTURE_H
