// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_TRIANGLE_H
#define ELUCIDO_TRIANGLE_H

#include "Object.h"

class Triangle : public Object {
 public:
//=============================================================================
// Data members
//=============================================================================
// Vertices have to be defined in counterclockwise direction,
// starting from vertex v0 going to v2 otherwise strange
// artefacts could occur
  glm::vec4 v0, v1, v2;       // vertices of the triangle
  glm::vec4 n;                // n of the triangle

//=============================================================================
// Constructors & Destructors
//=============================================================================
  Triangle() :
      Object(),
      v0(-1, -1, 0, 1),
      v1(1, -1, 0, 1),
      v2(0, 1, 0, 1) {
    n = glm::normalize(glm::vec4(glm::cross(glm::vec3(v1) - glm::vec3(v0),
                                            glm::vec3(v2) - glm::vec3(v0)),
                                 0));
    reshape_bb();
    ot = triangle;
  }

//=============================================================================
  Triangle(const material &m) :
      Object(m),
      v0(-1, -1, 0, 1),
      v1(1, -1, 0, 1),
      v2(0, 1, 0, 1) {
    n = glm::normalize(glm::vec4(glm::cross(glm::vec3(v1) - glm::vec3(v0),
                                            glm::vec3(v2) - glm::vec3(v0)),
                                 0));
    reshape_bb();
    ot = triangle;
  }

//=============================================================================
  Triangle(const glm::vec4 &vec0,
           const glm::vec4 &vec1,
           const glm::vec4 &vec2) :
      Object(),
      v0(vec0),
      v1(vec1),
      v2(vec2) {
    reshape_bb();
    ot = triangle;
  }

//=============================================================================
  Triangle(const glm::vec4 &vec0,
           const glm::vec4 &vec1,
           const glm::vec4 &vec2,
           const material &m) :
      Object(m),
      v0(vec0),
      v1(vec1),
      v2(vec2) {
    n = glm::normalize(glm::vec4(glm::cross(glm::vec3(v1) - glm::vec3(v0),
                                            glm::vec3(v2) - glm::vec3(v0)),
                                 0));
    reshape_bb();
    ot = triangle;
  }

//=============================================================================
  Triangle(const Triangle &t) : Object(t) {
    this->v0 = t.v0;
    this->v1 = t.v1;
    this->v2 = t.v2;
    this->n = t.n;
  }

//=============================================================================
  ~Triangle() {}


//=============================================================================
// Function declarations
//=============================================================================
  bool intersect(const Ray &r, isect_info &i) const;
  void get_surface_properties(isect_info &i) const;
  void apply_camera_transformation(const glm::mat4 &ivm);
  void apply_transformations();
  void translate(const float_t &translation,
                 const Axis &axes_of_translation);
  void rotate(const float_t &angle_of_rotation,
              const Axis &axes_of_rotation);
  void scale(const float_t &scaling_factor,
             const Axis &axes_of_scale);


 private:
  inline void reshape_bb() {
    // we don't want the bounding box to maintain its old shape
    bb.reset();

    // extend bb of triangle
    bb.extend_by(v0);
    bb.extend_by(v1);
    bb.extend_by(v2);
  }
};

#endif //ELUCIDO_TRIANGLE_H
