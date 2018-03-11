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
//=============================================================================
// Data members
//=============================================================================
  material om;
  AABBox bb;
  ObjectType ot;


//=============================================================================
// Constructors & destructors
//=============================================================================
  Object() : om(material()),
             mt(glm::mat4(1)),
             bb(AABBox()),
             ot(not_set_ot) {}

//=============================================================================
  Object(const material &m) : om(m),
                              mt(glm::mat4(1)),
                              bb(AABBox()),
                              ot(not_set_ot) {}

//=============================================================================
  Object(const Object &o);

//=============================================================================
  virtual ~Object() {}

//=============================================================================
// Function declarations
//=============================================================================
  virtual bool intersect(const Ray &r, isect_info &i) const = 0;
  virtual void get_surface_properties(isect_info &i) const = 0;
  virtual void apply_camera_transformation(const glm::mat4 &ctm) = 0;
  virtual void apply_transformations() = 0;
  void translate(const float_t &translation,
                 const Axis &translation_axis);
  void rotate(const float_t &angle_of_rotation,
              const Axis &rotation_axis);
  void scale(const float_t &scaling_factor,
             const Axis &scale_axis);
 protected:
  glm::mat4 mt;
};

#endif //ELUCIDO_OBJECT_H
