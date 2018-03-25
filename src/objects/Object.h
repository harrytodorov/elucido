// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_OBJECT_H
#define ELUCIDO_OBJECT_H

#include "../core/Ray.h"
#include "../accelerators/AABBox.h"
#include "../core/Utilities.h"

class Object {
//==============================================================================
// Constructors & destructors
//==============================================================================
 public:
  Object() :
      om(),
      mt(),
      bb(),
      ot(not_set_ot)
  {}
  Object(const material &m) :
      om(m),
      mt(),
      bb(),
      ot(not_set_ot)
  {}
  Object(const Object &o);

  virtual ~Object() {}

//==============================================================================
// Function declarations
//==============================================================================
  inline material const& material() const { return this->om; }
  inline void            set_material(const struct material &_m) { om = _m; }

  inline AABBox const& bounding_box() const { return this->bb; }

  inline ObjectType const& object_type() const { return this->ot; }
  inline void              set_object_type(const ObjectType &_ot) { ot = _ot; }

  virtual bool intersect(const Ray &r, isect_info &i) const = 0;

  virtual void apply_camera_transformation(const glm::mat4 &ctm) = 0;
  virtual void apply_transformations() = 0;
  virtual void translate(const float_t &translation,
                 const Axis &translation_axis);
  virtual void rotate(const float_t &angle_of_rotation,
              const Axis &rotation_axis);
  virtual void scale(const float_t &scaling_factor,
             const Axis &scale_axis);

//==============================================================================
// Data members
//==============================================================================
 protected:
  struct material   om{}; // Material.
  AABBox            bb{}; // Bounding box.
  ObjectType        ot{}; // Type.
  glm::mat4         mt{}; // Model transform matrix.
};

#endif //ELUCIDO_OBJECT_H
