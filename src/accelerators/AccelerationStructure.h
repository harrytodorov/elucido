// Copyright (c) 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_ACCELERATIONSTRUCTURE_H
#define ELUCIDO_ACCELERATIONSTRUCTURE_H

#include "../objects/Object.h"
#include "../objects/TriangleMesh.h"

struct Primitive {
//==============================================================================
// Constructors & destructors.
//==============================================================================
  Primitive() {}
  Primitive(const std::shared_ptr<Object> &_obj, const uint32_t &ti) :
      obj(_obj),
      tri_ind(ti)
  {}

  ~Primitive() {}

//==============================================================================
// Function declarations
//==============================================================================
  bool intersect(const Ray &r, isect_info &ii) const {
    if (obj->object_type() == triangle_mesh) {
      return std::static_pointer_cast<TriangleMesh>(obj)->intersect_triangle(r,
                                                                             tri_ind,
                                                                             ii);
    }
    return obj->intersect(r, ii);
  }

  const AABBox & getBB() {
    if (obj->object_type() != triangle_mesh) {
      return obj->bounding_box();
    }
    return std::static_pointer_cast<TriangleMesh>(obj)->get_BB(tri_ind);
  }

//==============================================================================
// Data members
//==============================================================================
  std::shared_ptr<Object> obj;
  uint32_t                tri_ind;
};

class AccelerationStructure {
//==============================================================================
// Constructors & destructors.
//==============================================================================
 public:
  AccelerationStructure(const AABBox &box,
                        const std::vector<std::shared_ptr<Object>> &objects) :
  bbox(box) {
    for (auto const &object : objects) {
      std::vector<Primitive> toPrimitive = convert_to_primitive(object);
      primitives.reserve(primitives.size() + toPrimitive.size());
      primitives.insert(primitives.end(),
                        toPrimitive.begin(),
                        toPrimitive.end());
    }
  }

  ~AccelerationStructure() {}

//==============================================================================
// Function declarations
//==============================================================================
  /**
   * Converts an object (sphere, triangle or triangle mesh) into a vector
   * of primitives.
   * @param obj:    The object to be converted.
   * @return:       The compound primitives of the object.
   */
  std::vector<Primitive> convert_to_primitive(const std::shared_ptr<Object> &obj) const;

  virtual bool intersect(const Ray &r, isect_info &i) const = 0;

//==============================================================================
// Data members
//==============================================================================
  AABBox                  bbox{};
  std::vector<Primitive>  primitives;
};

#endif //ELUCIDO_ACCELERATIONSTRUCTURE_H
