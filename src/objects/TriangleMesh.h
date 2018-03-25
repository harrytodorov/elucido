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
  TriangleMesh(const struct material &m) : Object(m) {
    ot = triangle_mesh;
  }
  TriangleMesh(const char *f) : Object() {
    load_mesh(f);
    ot = triangle_mesh;
  }
  TriangleMesh(const TriangleMesh &tm);

  ~TriangleMesh() = default;

//==============================================================================
// Function declarations
//==============================================================================
  bool intersect(const Ray &r, isect_info &i) const;
  bool intersect_triangle(const Ray &r,
                          const uint32_t &ti,
                          isect_info &i) const;

  /**
   * Computes the normal for the intersection point defined in i.
   * @param i:  A structure containing intersection information.
   */
  void compute_normal(isect_info &i) const;
  const AABBox & get_BB(const uint32_t &ti) const;
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
