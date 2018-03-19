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

  /**
   * One uses the Moeller-Trumbore ray/triangle intersection algorithm for
   * double faced triangles.
   * @param r:  The ray with which the triangle would be intersected.
   * @param i:  A structure containing intersection information.
   * @return:   True in case of an intersection, false otherwise.
   *            In case the ray runs parallel to the surface of the
   *            triangle, false is returned.
   */
  bool intersect(const Ray &r, isect_info &i) const;

  /**
   * Same algorithm is used for the shadow intersection routine.
   * @param r:  The ray with which the triangle would be intersected.
   * @return:   True in case of an intersection, false otherwise.
   *            In case the ray runs parallel to the surface of the
   *            triangle, false is returned.
   */
  bool shadow_intersect(const Ray &r) const;

  /**
   * Calculates the normal of the triangle:
   *  n = (v1 - v0) x (v2 - v0)
   */
  void calculate_normal();
  void reshape_bb();

  void apply_camera_transformation(const glm::mat4 &ivm);
  void apply_transformations();

//==============================================================================
// Data members
//==============================================================================
 protected:
  glm::vec4 v0, v1, v2;       // Vertices.
  glm::vec4 n;                // Normal.
};

#endif //ELUCIDO_TRIANGLE_H
