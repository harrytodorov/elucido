// Copyright (c) 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_ACCELERATIONSTRUCTURE_H
#define ELUCIDO_ACCELERATIONSTRUCTURE_H

#include "../objects/Object.h"
#include "../objects/TriangleMesh.h"

class AccelerationStructure {
  struct Primitive {
    Primitive(const Object *_object, const uint32_t &_ti) : object(_object),
                                                            ti(_ti) {}
    const Object *object; // The object itself.
    uint32_t ti;          // Triangle index, in case the primitive
                          // is TriangleMesh.

    bool intersect(const Ray &r, isect_info &i) {
      bool intesection{false};
      if (object->ot == triangle_mesh) {
        intesection = static_cast<TriangleMesh>(*object).intersect(r, ti, i);
      } else {
        intesection = object->intersect(r, i);
      }

    }

  };
};

#endif //ELUCIDO_ACCELERATIONSTRUCTURE_H
