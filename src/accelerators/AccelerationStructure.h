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

  const AABBox & getBB() const {
    if (obj->object_type() != triangle_mesh) {
      return obj->bounding_box();
    }
    return std::static_pointer_cast<TriangleMesh>(obj)->get_BB(tri_ind);
  }

  glm::vec4 centroid() const {
    return obj->centroid(tri_ind);
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
  AccelerationStructure() = default;

  virtual ~AccelerationStructure() {}

//==============================================================================
// Function declarations
//==============================================================================
  /**
   * Converts an object (sphere, triangle or triangle mesh) into a vector
   * of primitives. Primitives vector is emptied before filling.
   * @param obj:    The object to be converted.
   * @return:       The compound primitives of the object.
   */
  std::vector<Primitive> convert_to_primitive(const std::shared_ptr<Object> &obj) const;
  inline const std::vector<Primitive> & get_primitives() const {
    return primitives;
  };
  inline void compute_primitives(const uint32_t &number_primitives,
                                 const std::vector<std::shared_ptr<Object>> &objects) {
    // Clear existing primitives.
    if (!primitives.empty()) primitives.clear();
    primitives.reserve(number_primitives);
    for (auto const &object : objects) {
      std::vector<Primitive> toPrimitive = convert_to_primitive(object);
      primitives.insert(primitives.end(),
                        toPrimitive.begin(),
                        toPrimitive.end());
    }
  };

  inline const AccelerationStructureType & get_type() const { return as_type; }

  virtual bool traverse(const Ray &r, isect_info &i) const = 0;
  virtual void construct(const AABBox &box,
                         const std::vector<std::shared_ptr<Object>> &objects,
                         const uint32_t &number_primitives,
                         as_construct_info &info) = 0;

//==============================================================================
// Data members
//==============================================================================
 protected:
  AABBox                      bbox{};
  std::vector<Primitive>      primitives{};
  AccelerationStructureType   as_type{not_set_act};
};

#endif //ELUCIDO_ACCELERATIONSTRUCTURE_H
