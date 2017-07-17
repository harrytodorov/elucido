//
// Created by Haralambi Todorov on 05/07/2017.
//

#ifndef ELUCIDO_TRIANGLEMESH_H
#define ELUCIDO_TRIANGLEMESH_H

#include "Object.h"

class TriangleMesh : public Object {

public:
    std::vector<glm::vec4> va;  // vertex array
    std::vector<glm::vec4> vna; // vertex normal array
    std::vector<uint32_t> via;  // vertex index array
    std::vector<uint32_t> vnia; // vertex normal index array
    uint32_t nt{0};             // number of triangles in the mesh
    uint32_t nf{0};             // number of faces in the mesh

    TriangleMesh() : Object() {}
    TriangleMesh(Material *m) : Object(m) {}
    TriangleMesh(const char *f) : Object() {
        load_mesh(f);
    }
    virtual ~TriangleMesh() {}

    bool intersect(const Ray &r, isect_info &i);
    void get_surface_properties(isect_info &i) const;
    void apply_camera_transformation(const glm::mat4 &ctm, const glm::mat4 &tictm);
    void apply_transformations();
    void translate(const float_t &translation, const uint32_t &axes_of_translation);
    void rotate(const float_t &angle_of_rotation, const uint32_t &axes_of_rotation);
    void scale(const float_t &scaling_factor, const uint32_t &axes_of_scale);
    loading_info load_mesh(const char *f);
    bool triangle_intersect(const Ray &r, const glm::vec4 &v0, const glm::vec4 &v1, const glm::vec4 &v2,
                                float_t &t, float_t &u, float_t &v);
};

#endif //ELUCIDO_TRIANGLEMESH_H
