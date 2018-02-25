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

#include "Ray.h"
#include "AABBox.h"
#include "Utilities.h"

class Object {
 public:
//=============================================================================
// Data members
//=============================================================================
  material om;        // object's material
  AABBox bb;          // bounding box of the object
  ObjectType ot;      // object's type


//=============================================================================
// Constructors & destructors
//=============================================================================
  Object() : om(material()), mt(glm::mat4(1)), bb(AABBox()), ot(def) {}

//=============================================================================
  Object(const material &m) : om(m), mt(glm::mat4(1)), nmt(glm::mat4(1)),
                              bb(AABBox()), ot(def) {}

//=============================================================================
  Object(const Object &o);

//=============================================================================
  virtual ~Object() {}

//=============================================================================
// Function declarations
//=============================================================================
  virtual bool intersect(const Ray &r, isect_info &i) const = 0;
  virtual void get_surface_properties(isect_info &i) const = 0;
  virtual void apply_camera_transformation(const glm::mat4 &ctm,
                                           const glm::mat4 &tictm) = 0;
  virtual void apply_transformations() = 0;
  virtual void translate(const float_t &translation,
                         const Axis &axes_of_translation) = 0;
  virtual void rotate(const float_t &angle_of_rotation,
                      const Axis &axes_of_rotation) = 0;
  virtual void scale(const float_t &scaling_factor,
                     const Axis &axes_of_scale) = 0;

 protected:
  glm::mat4 mt;     // model transform matrix for an object
  // places and transforms the object in world coordinate space
  glm::mat4 nmt;    // transformation matrix for object's normals
};

#endif //ELUCIDO_OBJECT_H
