//
// Created by Haralambi Todorov on 05/07/2017.
//

#ifndef ELUCIDO_TRIANGLEMESH_H
#define ELUCIDO_TRIANGLEMESH_H

#include <cstdint>
#include <glm/vec4.hpp>
#include "Triangle.h"

class TriangleMesh : public Object {
public:
    std::vector<Triangle> ts;

    TriangleMesh() : Object() {}
    TriangleMesh(Material *m) : Object(m) {}
    TriangleMesh(const char *f) : Object() {
        load_mesh(f);
    }
    virtual ~TriangleMesh() {ts.clear();}

    bool intersect(const Ray &r, float_t &t, glm::vec4 &p_hit, uint32_t &ti);
    void get_surface_properties(const glm::vec4 &hit_point, const glm::vec4 &view_direction, const uint32_t &triangle_index,
                                    glm::vec4 &hit_normal);
    void apply_camera_transformation(glm::mat4 &t);
    void apply_transformations();
    void translate(const float_t &translation, const uint32_t &axes_of_translation);
    void rotate(const float_t &angle_of_rotation, const uint32_t &axes_of_rotation);
    void scale(const float_t &scaling_factor, const uint32_t &axes_of_scale);
    loading_info load_mesh(const char *f);
};

#endif //ELUCIDO_TRIANGLEMESH_H
