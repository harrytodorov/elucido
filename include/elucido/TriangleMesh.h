// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_TRIANGLEMESH_H
#define ELUCIDO_TRIANGLEMESH_H

#include "Object.h"


class TriangleMesh : public Object {

 public:
//=============================================================================
// Data members
//=============================================================================
  std::vector<glm::vec4> va;          // vertex array
  std::vector<glm::vec4> vna;         // vertex normal array
  std::vector<uint32_t> via;          // vertex index array
  std::vector<uint32_t> vnia;         // vertex normal index array
  uint32_t nt{0};                     // number of triangles in the mesh
  uint32_t nf{0};                     // number of faces in the mesh
  bool in{false};                     // interpolate normals

//=============================================================================
// Constructors & destructors
//=============================================================================
  TriangleMesh() {
    ot = triangle_mesh;
  }

//=============================================================================
  TriangleMesh(const material &m, const bool &in) : Object(m), in(in) {
    ot = triangle_mesh;
  }

//=============================================================================
  explicit TriangleMesh(const material &m) : Object(m) {
    ot = triangle_mesh;
  }

//=============================================================================
  explicit TriangleMesh(const char *f) : Object() {
    load_mesh(f);
    ot = triangle_mesh;
  }

//=============================================================================
  TriangleMesh(const TriangleMesh &tm);

//=============================================================================
  ~TriangleMesh() = default;


//=============================================================================
// Function declarations
//=============================================================================
  bool intersect(const Ray &r, isect_info &i) const;
  bool intersect(const Ray &r, const uint32_t &ti, isect_info &i) const;
  bool triangle_intersect(const Ray &r,
                          const glm::vec4 &v0,
                          const glm::vec4 &v1,
                          const glm::vec4 &v2,
                          float_t &t,
                          float_t &u,
                          float_t &v) const;
  void get_surface_properties(isect_info &i) const;
  void apply_camera_transformation(const glm::mat4 &ctm,
                                   const glm::mat4 &tictm);
  void apply_transformations();
  void translate(const float_t &translation,
                 const Axis &axes_of_translation);
  void rotate(const float_t &angle_of_rotation,
              const Axis &axes_of_rotation);
  void scale(const float_t &scaling_factor, const Axis &axes_of_scale);
  loading_info load_mesh(const char *f);
  const AABBox* getBoundingBoxForTriangle(const uint32_t &ti) const;
};

#endif //ELUCIDO_TRIANGLEMESH_H
