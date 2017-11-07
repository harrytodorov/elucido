// Copyright (c) 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_ACCELERATIONSTRUCTURE_H
#define ELUCIDO_ACCELERATIONSTRUCTURE_H

#include "Object.h"
#include "TriangleMesh.h"

class AccelerationStructure {
  struct Primitive {
    Primitive(const Object *_object, const uint32_t &_ti) : object(_object),
                                                            ti(_ti) {}
    const Object *object; // The object itself.
    uint32_t ti;          // Triangle index, in case the primitive
                          // is TriangleMesh.
  };
};

#endif //ELUCIDO_ACCELERATIONSTRUCTURE_H
