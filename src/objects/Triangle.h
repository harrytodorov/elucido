// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_TRIANGLE_H
#define ELUCIDO_TRIANGLE_H

#include "Object.h"

class Triangle : public Object {
//==============================================================================
// Constructors & Destructors
//==============================================================================
 public:
  Triangle() :
      Object(),
      v0(-1, -1, 0, 1),
      v1(1, -1, 0, 1),
      v2(0, 1, 0, 1) {
    calculate_normal();
    reshape_bb();
    ot = triangle;
  }
  Triangle(const glm::vec4 &vec0,
           const glm::vec4 &vec1,
           const glm::vec4 &vec2) :
      Object(),
      v0(vec0),
      v1(vec1),
      v2(vec2) {
    calculate_normal();
    reshape_bb();
    ot = triangle;
  }
  Triangle(const Triangle &t) : Object(t) {
    this->v0 = t.v0;
    this->v1 = t.v1;
    this->v2 = t.v2;
    this->n = t.n;
  }

  ~Triangle() {}

//==============================================================================
// Function declarations
//==============================================================================
  inline glm::vec4 vert0() const { return this->v0; }
  inline glm::vec4 vert1() const { return this->v1; }
  inline glm::vec4 vert2() const { return this->v2; }
  inline void      set_vertices(const glm::vec4 &_v0,
                                const glm::vec4 &_v1,
                                const glm::vec4 &_v2) {
    this->v0 = _v0;
    this->v1 = _v1;
    this->v2 = _v2;
    calculate_normal();
  }

  inline glm::vec4 normal() const { return this->n; }

  bool intersect(const Ray &r, isect_info &i) const;
  bool shadow_intersect(const Ray &r) const;

  void get_surface_properties(isect_info &i) const;
  void apply_camera_transformation(const glm::mat4 &ivm);
  void apply_transformations();
  void calculate_normal();
  void reshape_bb();

//==============================================================================
// Data members
//==============================================================================
 protected:
  glm::vec4 v0, v1, v2;       // Vertices.
  glm::vec4 n;                // Normal.
};

#endif //ELUCIDO_TRIANGLE_H
