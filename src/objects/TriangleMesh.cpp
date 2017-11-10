// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "../../include/elucido/TriangleMesh.h"

//=============================================================================
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
      float_t vnx, vny,
          vnz;          // x, y and z components of the vertex normal
      tokens >> vnx >> vny >> vnz;

      // add vertex normal to temporary vertex normal array
      vna.push_back(glm::vec4(vnx, vny, vnz, 0));

      // in case we have a face
    } else if (type == face) {
      std::string fp;                 // temporary holder for face information
      std::string d = "/";            // d: d
      std::string vd,
          cv;             // vd: vertex dummy, used to store information for the extracted vertex,
      // vertex normal or vertex texture
      // cv: current vertex
      std::vector<uint32_t> vertex_index, vertexnormal_index;
      uint32_t nv(0);                 // number of vertices in the face

      // extract  information
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

  ret.nv = (uint32_t) va.size();
  ret.nvn = (uint32_t) vna.size();
  ret.nf = nf;
  ret.nt = nt;

  return ret;
}

//=============================================================================
bool TriangleMesh::triangle_intersect(const Ray &r,
                                      const glm::vec4 &v0,
                                      const glm::vec4 &v1,
                                      const glm::vec4 &v2,
                                      float_t &t,
                                      float_t &u,
                                      float_t &v) const {
  // define the two edges of the triangle AB and AC
  glm::vec4 e0(v1 - v0), e1(v2 - v0), cv(r.orig() - v0);
  glm::vec4 pv, qv;
  float_t det, inv_det;

  // calculate the p vector from MT, used for calculating the determinant and u parameter
  pv = glm::vec4(glm::cross(glm::vec3(r.dir()), glm::vec3(e1)), 0);

  // calculate the determinant of the 1x3 matrix M
  det = glm::dot(pv, e0);

  // if determinant is near 0, ray is parallel to the triangle
  if (det < kEpsilon)
    return false;

  // calculate the inverse determinant
  inv_det = 1.f / det;

  // calculate u parameter and test for its bounds
  u = glm::dot(pv, cv) * inv_det;
  if (u < 0.f || u > 1.f)
    return false;

  // calculate q vector from MT, used for calculating v parameter
  qv = glm::vec4(glm::cross(glm::vec3(cv), glm::vec3(e0)), 0);

  // calculate v parameter and test for its bounds
  v = glm::dot(qv, r.dir()) * inv_det;
  if (v < 0.f || (v + u > 1.f))
    return false;

  // calculate t
  t = glm::dot(qv, e1) * inv_det;

  return t > kEpsilon;
}

//=============================================================================
bool TriangleMesh::intersect(const Ray &r,
                             const uint32_t &ti,
                             isect_info &i) const {
  glm::vec4 v0, v1, v2;
  bool intersected{false};

  // get the vertex information for the triangle
  v0 = va[via[3 * ti] - 1];
  v1 = va[via[3 * ti + 1] - 1];
  v2 = va[via[3 * ti + 2] - 1];

  float_t tt{infinity}, u{0}, v{0};

  // intersection test
  if (triangle_intersect(r, v0, v1, v2, tt, u, v) && tt < i.tn) {
    intersected = true;
    i.tn = tt;
    i.u = u;
    i.v = v;
    i.ip = r.orig() + tt * r.dir();
    i.ti = ti;
  }

  return intersected;
}

//=============================================================================
bool TriangleMesh::intersect(const Ray &r, isect_info &i) const {
  glm::vec4 v0, v1, v2;
  bool intersected{false};

  // iterate through triangles in the mesh
  for (uint32_t _ti = 0; _ti < nt; _ti++) {

    // intersection test
    if (intersect(r, _ti, i)) {
      intersected = true;
    }
  }

  return intersected;
}

//=============================================================================
void TriangleMesh::get_surface_properties(isect_info &i) const {

  if (in) {
    glm::vec4 vn0, vn1, vn2;
    float_t w, u, v;

    // get the vertex normals for the current triangle
    vn0 = vna[vnia[3 * i.ti] - 1];
    vn1 = vna[vnia[3 * i.ti + 1] - 1];
    vn2 = vna[vnia[3 * i.ti + 2] - 1];

    // assign the barycentric coordinates for the current triangle intersection point
    u = i.u;
    v = i.v;
    w = 1.f - u - v;

    // normalize the vertex normal, we shouldn't need this, but just for safety
    i.ipn = glm::normalize(w * vn0 + u * vn1 + v * vn2);
  } else {
    glm::vec4 v0, v1, v2;

    // get the vertices for the current triangle
    v0 = va[via[3 * i.ti] - 1];
    v1 = va[via[3 * i.ti + 1] - 1];
    v2 = va[via[3 * i.ti + 2] - 1];

    i.ipn = glm::normalize(glm::vec4(glm::cross(glm::vec3(v1) - glm::vec3(v0),
                                                glm::vec3(v2) - glm::vec3(v0)),
                                     0));
  }
}

//=============================================================================
void TriangleMesh::apply_camera_transformation(const glm::mat4 &ctm,
                                               const glm::mat4 &tictm) {
  glm::vec4 v, vn;

  // reset the bounding box size
  bb.reset();

  // iterate through vertices in the mesh and apply transformation
  for (auto &_ti : va) {
    // get the vertex
    v = _ti;

    // apply the matrix transformation on the vertices
    v = ctm * v;

    bb.extend_by(v);

    // assign the transformed vertices
    _ti = v;
  }

  // iterate through vertex normals and apply transformations
  for (auto &_ti : vna) {
    // get the vertex normal
    vn = _ti;

    // apply the normal transformation matrix
    vn = glm::normalize(tictm * vn);

    // assign the transformed vertex normal
    _ti = vn;
  }
}

//=============================================================================
void TriangleMesh::apply_transformations() {
  glm::vec4 v, vn;

  // reset the bounding box size
  bb.reset();

  // iterate through vertices in the mesh and apply transformation
  for (auto &_ti : va) {
    // get the vertex
    v = _ti;

    // apply the matrix transformation on the vertices
    v = mt * v;

    bb.extend_by(v);

    // assign the transformed vertices
    _ti = v;
  }

  // iterate through vertex normals and apply transformations
  for (auto &_ti : vna) {
    // get the vertex normal
    vn = _ti;

    // apply the normal transformation matrix
    vn = glm::normalize(glm::transpose(glm::inverse(nmt)) * vn);

    // assign the transformed vertex normal
    _ti = vn;
  }

  // after applying the transformations to a triangle mesh; its model transform and normal transform matrices are
  // set back to the identity matrix
  mt = glm::mat4(1);
  nmt = glm::mat4(1);
}

//=============================================================================
void TriangleMesh::translate(const float_t &translation,
                             const uint32_t &axes_of_translation) {
  // create 3d vector to determine the axes of translation
  glm::vec3 tv(0);

  switch (axes_of_translation) {
    case X :tv.x = translation;
      break;
    case Y :tv.y = translation;
      break;
    case Z :tv.z = translation;
      break;
    case XY :tv.x = translation;
      tv.y = translation;
      break;
    case XZ :tv.x = translation;
      tv.z = translation;
      break;
    case YZ :tv.y = translation;
      tv.z = translation;
      break;
    case XYZ :tv = glm::vec3(translation);
      break;
    default:printf("You're using an undefined axis of translation.");
      break;
  }

  // assign the translation matrix to the object's model transform
  glm::mat4 tm = glm::translate(glm::mat4(1), tv);
  mt = tm * mt;
}

//=============================================================================
void TriangleMesh::rotate(const float_t &angle_of_rotation,
                          const uint32_t &axes_of_rotation) {
  // create 3d vector to determine the axis of rotation
  glm::vec3 rv(0);

  switch (axes_of_rotation) {
    case X :rv.x = 1;
      break;
    case Y :rv.y = 1;
      break;
    case Z :rv.z = 1;
      break;
    case XY :rv.x = 1;
      rv.y = 1;
      break;
    case XZ :rv.x = 1;
      rv.z = 1;
      break;
    case YZ :rv.y = 1;
      rv.z = 1;
      break;
    case XYZ :rv = glm::vec3(1);
      break;
    default:printf("You're using an undefined axis of rotation.");
      break;
  }

  // get the rotation matrix
  glm::mat4 rm = glm::rotate(glm::mat4(1), glm::radians(angle_of_rotation), rv);
  mt = rm * mt;
  nmt = rm * nmt;
}

//=============================================================================
void TriangleMesh::scale(const float_t &scaling_factor,
                         const uint32_t &axes_of_scale) {
  // create 3d vector to determine the axes of scale
  glm::vec3 sv(0);

  switch (axes_of_scale) {
    case X :sv.x = scaling_factor;
      break;
    case Y :sv.y = scaling_factor;
      break;
    case Z :sv.z = scaling_factor;
      break;
    case XY :sv.x = scaling_factor;
      sv.y = scaling_factor;
      break;
    case XZ :sv.x = scaling_factor;
      sv.z = scaling_factor;
      break;
    case YZ :sv.y = scaling_factor;
      sv.z = scaling_factor;
      break;
    case XYZ :sv = glm::vec3(scaling_factor);
      break;
    default:printf("You're using an undefined axis of scale.");
      break;
  }

  // assign the scale matrix to the object's model transform
  glm::mat4 sm = glm::scale(glm::mat4(1), sv);
  mt = sm * mt;
  nmt = sm * nmt;
}

//=============================================================================
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

//=============================================================================
const AABBox* TriangleMesh::getBoundingBoxForTriangle(const uint32_t &ti) const {
  AABBox *box = new AABBox();
  glm::vec4 v0, v1, v2;
  // get the vertices for the current triangle
  v0 = va[via[3 * ti] - 1];
  v1 = va[via[3 * ti + 1] - 1];
  v2 = va[via[3 * ti + 2] - 1];
  box->extend_by(v0);
  box->extend_by(v1);
  box->extend_by(v2);
  return box;
}