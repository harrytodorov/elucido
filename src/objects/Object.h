// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_OBJECT_H
#define ELUCIDO_OBJECT_H

#include <glm/detail/type_vec.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/vec4.hpp>

#include <cstdio>
#include <cmath>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdint>

#include "../extra/Ray.h"
#include "../accelerators/AABBox.h"
#include "../extra/Utilities.h"

class Object {
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

  inline material const& material() const { return this->om; }
  inline void            set_material(const struct material &_m) { om = _m; }

  inline AABBox const& bounding_box() const { return this->bb; }
  inline void          set_bounding_box(const AABBox &_bb) { bb = _bb; }

  inline ObjectType const& object_type() const { return this->ot; }
  inline void              set_object_type(const ObjectType &_ot) { ot = _ot; }

  virtual bool intersect(const Ray &r, isect_info &i) const = 0;
  virtual bool shadow_intersect(const Ray &r) const = 0;

  virtual void get_surface_properties(isect_info &i) const;
  virtual void apply_camera_transformation(const glm::mat4 &ctm) = 0;
  virtual void apply_transformations() = 0;
  virtual void translate(const float_t &translation,
                 const Axis &translation_axis);
  virtual void rotate(const float_t &angle_of_rotation,
              const Axis &rotation_axis);
  virtual void scale(const float_t &scaling_factor,
             const Axis &scale_axis);
 protected:
  struct material   om{}; // Material.
  AABBox            bb{}; // Bounding box.
  ObjectType        ot{}; // Type.
  glm::mat4         mt{}; // Model transform matrix.
};

#endif //ELUCIDO_OBJECT_H
