// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "TriangleMesh.h"

//==============================================================================
loading_info TriangleMesh::load_mesh(const char *f) {
  std::string type;               // type of the object read on the current line
  std::string line;               // one line of the file
  std::vector<glm::vec4> tva;     // temporary array to store vertices
  std::vector<glm::vec4> tvna;    // temporary array to store vertex normals
  std::vector<uint32_t> vi;       // vertex index array
  std::vector<uint32_t> ni;      // normal index array
  loading_info ret;               // holder for the loading information

  // load file in memory
  std::ifstream fs(f);

  // if we fail to load file from memory, exit the function
  if (!fs.good())
    return ret;

  // read the file line by line
  while (std::getline(fs, line)) {
    // convert the line into tokenizable object
    std::stringstream tokens(line);

    // read what is the type of the object that had to be initialized
    // v:   vector
    // vn:  vector normal
    // f:   face
    tokens >> type;

    // in case we have a vertex
    if (type == vertex) {
      // x, y and z components of the vertex
      float_t vx, vy, vz;
      tokens >> vx >> vy >> vz;

      // extend bb of triangulated mesh
      bb.extend_by(glm::vec4(vx, vy, vz, 1));

      // add vertex to temporary vertex array
      va.push_back(glm::vec4(vx, vy, vz, 1));

      // in case we have a vertex normal
    } else if (type == vertex_normal) {
      float_t vnx, vny, vnz;
      tokens >> vnx >> vny >> vnz;

      // add vertex normal to temporary vertex normal array
      vna.push_back(glm::vec4(vnx, vny, vnz, 0));

      // in case we have a face
    } else if (type == face) {
      std::string fp;
      std::string d = "/";
      std::string vd, // vd: vertex dummy, used to store information for the extracted vertex,
                  cv; // cv: current vertex
      std::vector<uint32_t> vertex_index, vertexnormal_index;
      uint32_t nv(0);                 // number of vertices in the face

      // extract information
      while (tokens >> fp) {
        // extract vertex index information
        cv = fp;
        vd = cv.substr(0, cv.find(d));
        vertex_index.push_back((unsigned int) std::stoi(vd));

        // extract vertex texture information
        // TODO: for now we just skip this information, if any
        cv.erase(0, cv.find(d) + d.length());
        vd = cv.substr(0, cv.find(d));

        // extract vertex normal information
        cv.erase(0, cv.find(d) + d.length());
        vd = cv.substr(0, cv.find(d));
        vertexnormal_index.push_back((unsigned int) std::stoi(vd));

        nv++;
      }

      // generate 1 or multiple triangles for each face; depending on vertices number
      // each face has #_of_vertices - 2 triangles
      for (int t = 0; t < (nv - 2); t++) {
        // vertex indices for the triangle
        uint32_t v0, v1, v2;
        v0 = vertex_index.at(0);
        v1 = vertex_index.at((uint32_t) t + 1);
        v2 = vertex_index.at((uint32_t) t + 2);
        via.push_back(v0);
        via.push_back(v1);
        via.push_back(v2);

        // vertex normal indices for the triangle
        uint32_t vn0, vn1, vn2;
        vn0 = vertexnormal_index.at(0);
        vn1 = vertexnormal_index.at((uint32_t) t + 1);
        vn2 = vertexnormal_index.at((uint32_t) t + 2);
        vnia.push_back(vn0);
        vnia.push_back(vn1);
        vnia.push_back(vn2);

        nt++;
      }

      nf++;
    }
  }

  // close file after the data from it is extracted
  if (fs.is_open()) {
    fs.close();
  }

  ret.nv = (uint32_t) va.size();
  ret.nvn = (uint32_t) vna.size();
  ret.nf = nf;
  ret.nt = nt;

  return ret;
}

//==============================================================================
bool TriangleMesh::intersect(const Ray &r, isect_info &i) const {
  bool intersected{false};

  for (uint32_t _ti = 0; _ti < nt; _ti++) {
    if (intersect_triangle(r, _ti, i)) {
      intersected = true;
    }
  }

  // Compute the normal when there was an intersection.
  if (intersected) compute_normal(i);

  return intersected;
}

//==============================================================================
bool TriangleMesh::intersect_triangle(const Ray &r,
                                      const uint32_t &ti,
                                      isect_info &i) const {
  glm::vec4 v0, v1, v2;
  bool intersected{false};

  v0 = va[via[3 * ti] - 1];
  v1 = va[via[3 * ti + 1] - 1];
  v2 = va[via[3 * ti + 2] - 1];

  float_t tt{infinity}, u{0}, v{0};

  if (intersection_routine(r, v0, v1, v2, tt, u, v) && tt < i.tn) {
    intersected = true;
    i.tn = tt;
    i.u = u;
    i.v = v;
    i.ip = r.orig() + tt * r.dir();
    i.ti = ti;
  }

  return intersected;
}

//==============================================================================
bool TriangleMesh::intersection_routine(const Ray &r,
                                        const glm::vec4 &v0,
                                        const glm::vec4 &v1,
                                        const glm::vec4 &v2,
                                        float_t &t,
                                        float_t &u,
                                        float_t &v) const {

  auto edge1 = v1 - v0;
  auto edge2 = v2 - v0;

  auto p = glm::vec4(glm::cross(glm::vec3(r.dir()), glm::vec3(edge2)), 0.f);
  auto determinant = glm::dot(edge1, p);

  if (determinant > -kEpsilon && determinant < kEpsilon)
    return false;

  auto inv_determinant = 1.f / determinant;

  // Calculate distance vector from vertex 0 to the ray origin.
  auto d = r.orig() - v0;

  // Calculate Barycentric u-parameter.
  u = static_cast<float_t>(glm::dot(d, p) * inv_determinant);

  // Test if u-parameter is in the bounds [0,1].
  if (u < 0.f || u > 1.f) return false;

  auto q = glm::vec4(glm::cross(glm::vec3(d), glm::vec3(edge1)), 0.f);

  // Calculate Barycentric v-parameter.
  v = static_cast<float_t>(glm::dot(r.dir(), q) * inv_determinant);

  // Test if v-parameter is in the bounds [0,1].
  if (v < 0.f || v > 1.f) return false;

  // Calculate t.
  t = static_cast<float_t>(glm::dot(edge2, q) * inv_determinant);
}

//==============================================================================
bool TriangleMesh::shadow_intersect(const Ray &r) const {
  for (uint32_t _ti = 0; _ti < nt; _ti++) {
    glm::vec4 v0, v1, v2;

    v0 = va[via[3 * _ti] - 1];
    v1 = va[via[3 * _ti + 1] - 1];
    v2 = va[via[3 * _ti + 2] - 1];
    
    if (shadow_intersection_routine(r, v0, v1, v2)) {
      return true;
    }
  }
  
  return false;
}

//==============================================================================
bool TriangleMesh::shadow_intersection_routine(const Ray &r,
                                               const glm::vec4 &v0,
                                               const glm::vec4 &v1,
                                               const glm::vec4 &v2) const {
  auto edge1 = v1 - v0;
  auto edge2 = v2 - v0;

  auto p = glm::vec4(glm::cross(glm::vec3(r.dir()), glm::vec3(edge2)), 0.f);
  auto determinant = glm::dot(edge1, p);

  if (determinant > -kEpsilon && determinant < kEpsilon) return false;

  auto inv_determinant = 1.f / determinant;

  // Calculate distance vector from vertex 0 to the ray origin.
  auto d = r.orig() - v0;

  // Calculate Barycentric u-parameter.
  auto u = glm::dot(d, p) * inv_determinant;

  // Test if u-parameter is in the bounds [0,1].
  if (u < 0.f || u > 1.f) return false;

  auto q = glm::vec4(glm::cross(glm::vec3(d), glm::vec3(edge1)), 0.f);

  // Calculate Barycentric v-parameter.
  auto v = glm::dot(r.dir(), q) * inv_determinant;

  // Test if v-parameter is in the bounds [0,1].
  if (v < 0.f || v > 1.f) return false;

  return true;
}

//==============================================================================
void TriangleMesh::compute_normal(isect_info &i) const {
  if (in) {
    glm::vec4 vn0, vn1, vn2;
    float_t w, u, v;

    vn0 = vna[vnia[3 * i.ti] - 1];
    vn1 = vna[vnia[3 * i.ti + 1] - 1];
    vn2 = vna[vnia[3 * i.ti + 2] - 1];

    u = i.u;
    v = i.v;
    w = 1.f - u - v;

    i.ipn = w * vn0 + u * vn1 + v * vn2;
  } else {
    glm::vec4 v0, v1, v2;

    v0 = va[via[3 * i.ti] - 1];
    v1 = va[via[3 * i.ti + 1] - 1];
    v2 = va[via[3 * i.ti + 2] - 1];

    auto c = glm::cross(glm::vec3(v1 - v0),
                        glm::vec3(v2 - v0));

    i.ipn = glm::normalize(glm::vec4(c.x, c.y, c.z, 0.f));
  }
}

//==============================================================================
void TriangleMesh::apply_camera_transformation(const glm::mat4 &ctm) {
  glm::vec4 v, vn;
  bb.reset();

  for (auto &_ti : va) {
    v = _ti;
    v = ctm * v;
    bb.extend_by(v);
    _ti = v;
  }

  for (auto &_ti : vna) {
    vn = _ti;
    vn = glm::transpose(glm::inverse(ctm)) * vn;
    // Reset normal's w component to 0.
    vn.w = 0.f;
    // Renormalize.
    vn = glm::normalize(vn);
    _ti = vn;
  }
}

//==============================================================================
void TriangleMesh::apply_transformations() {
  glm::vec4 v, vn;
  bb.reset();

  // Transform vertices.
  for (auto &_ti : va) {
    v = _ti;
    v = mt * v;
    bb.extend_by(v);
    _ti = v;
  }

  // Transform vertex normals.
  for (auto &_ti : vna) {
    vn = _ti;
    vn = glm::transpose(glm::inverse(mt)) * vn;
    // Reset normal's w component to 0.
    vn.w = 0.f;
    // Renormalize.
    vn = glm::normalize(vn);
    _ti = vn;
  }

  // Reset model transform matrix.
  mt = glm::mat4(1);
}

//==============================================================================
TriangleMesh::TriangleMesh(const TriangleMesh &tm) : Object(tm) {
  this->va = tm.va;
  this->vna = tm.vna;
  this->via = tm.via;
  this->vnia = tm.vnia;
  this->nt = tm.nt;
  this->nf = tm.nf;
  this->in = tm.in;
  this->ot = tm.ot;
}

//==============================================================================
const AABBox* TriangleMesh::get_BB(const uint32_t &ti) const {
  auto *box = new AABBox();

  glm::vec4 v0, v1, v2;
  v0 = va[via[3 * ti] - 1];
  v1 = va[via[3 * ti + 1] - 1];
  v2 = va[via[3 * ti + 2] - 1];
  box->extend_by(v0);
  box->extend_by(v1);
  box->extend_by(v2);
  return box;
}