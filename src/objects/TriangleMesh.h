// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_TRIANGLEMESH_H
#define ELUCIDO_TRIANGLEMESH_H

#include "Object.h"

class TriangleMesh : public Object {
//==============================================================================
// Constructors & destructors
//==============================================================================
 public:
  TriangleMesh() {
    ot = triangle_mesh;
  }
  TriangleMesh(const struct material &m, const bool &in) : Object(m), in(in) {
    ot = triangle_mesh;
  }
  explicit TriangleMesh(const struct material &m) : Object(m) {
    ot = triangle_mesh;
  }
  explicit TriangleMesh(const char *f) : Object() {
    load_mesh(f);
    ot = triangle_mesh;
  }
  TriangleMesh(const TriangleMesh &tm);

  ~TriangleMesh() = default;

//==============================================================================
// Function declarations
//==============================================================================
  /**
   * One uses the Moeller-Trumbore ray/triangle intersection algorithm for
   * double faced triangles.
   * @param r:  The ray with which the triangle would be intersected.
   * @param v0: Vertex 0.
   * @param v1: Vertex 1.
   * @param v2: Vertex 2.
   * @param t:  Distance from the intersection point to the ray's origin.
   * @param u:  Barycentric u-parameter.
   * @param v:  Barycentric v-parameter.
   * @return:   True in case of an intersection, false otherwise.
   *            In case the ray runs parallel to the surface of the
   *            triangle, false is returned.
   */
  bool intersection_routine(const Ray &r,
                            const glm::vec4 &v0,
                            const glm::vec4 &v1,
                            const glm::vec4 &v2,
                            float_t &t,
                            float_t &u,
                            float_t &v,
                            bool &flip_normal) const;
  bool intersect(const Ray &r, isect_info &i) const;
  bool intersect_triangle(const Ray &r,
                          const uint32_t &ti,
                          isect_info &i) const;

  /**
   * Same algorithm is used for the shadow intersection routine.
   * @param r:  The ray with which the triangle would be intersected.
   * @param v0: Vertex 0.
   * @param v1: Vertex 1.
   * @param v2: Vertex 2.
   * @return:   True in case of an intersection, false otherwise.
   *            In case the ray runs parallel to the surface of the
   *            triangle, false is returned.
   */
  bool shadow_intersection_routine(const Ray &r,
                                     const glm::vec4 &v0,
                                     const glm::vec4 &v1,
                                     const glm::vec4 &v2) const;
  bool shadow_intersect(const Ray &r) const;

  /**
   * Computes the normal for the intersection point defined in i.
   * @param i:  A structure containing intersection information.
   */
  void compute_normal(isect_info &i) const;
  const AABBox* get_BB(const uint32_t &ti) const;
  loading_info load_mesh(const char *f);

  void apply_camera_transformation(const glm::mat4 &ctm);
  void apply_transformations();


//==============================================================================
// Data members
//==============================================================================
  std::vector<glm::vec4>  va;         // Vertex array.
  std::vector<glm::vec4>  vna;        // Vertex normal array.
  std::vector<uint32_t>   via;        // Vertex index array.
  std::vector<uint32_t>   vnia;       // Vertex normal index array.
  uint32_t                nt{0};      // Number of triangles.
  uint32_t                nf{0};      // Number of faces.
  bool                    in{false};  // Interpolate normals.
};

#endif //ELUCIDO_TRIANGLEMESH_H
