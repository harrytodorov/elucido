//
// Created by Haralambi Todorov on 19/05/2017.
//

#ifndef ELUCIDO_SPHERE_H
#define ELUCIDO_SPHERE_H

#include <glm/geometric.hpp>
#include "Object.h"

class Sphere : public Object {
    float_t     r;  // sphere's radius
    float_t     r2; // the squared sphere's radius; needed to compute the ray-sphere intersecion
                    // computed once, by constructing the sphere
    glm::vec4   c;  // sphere's center

public:
    Sphere() : Object(), r(1.0), c(0, 0, 0, 1) {
        r2 = 1.f;
        bb.extend_by(glm::vec4(c.x-r, c.y-r, c.z-r, 1));
        bb.extend_by(glm::vec4(c.x+r, c.y+r, c.z+r, 1));
    }
    Sphere(Material *mat) : Object(mat),  r(1.0), c(0, 0, 0, 1) {
        r2 = 1.f;
        bb.extend_by(glm::vec4(c.x-r, c.y-r, c.z-r, 1));
        bb.extend_by(glm::vec4(c.x+r, c.y+r, c.z+r, 1));
    }
    Sphere(const glm::vec4 &_c, const float_t &_r) : Object(), r(_r), c(_c) {
        r2 = powf(_r, 2.f);
        bb.extend_by(glm::vec4(c.x-r, c.y-r, c.z-r, 1));
        bb.extend_by(glm::vec4(c.x+r, c.y+r, c.z+r, 1));
    }
    Sphere(const glm::vec4 &c, const float_t &r, Material *m) : Object(m), r(r), c(c) {
        r2 = powf(r, 2.f);
        bb.extend_by(glm::vec4(c.x-r, c.y-r, c.z-r, 1));
        bb.extend_by(glm::vec4(c.x+r, c.y+r, c.z+r, 1));
    }
    ~Sphere() {}

    void set_radius(const float_t &r);
    void set_center_p(const glm::vec4 &p);
    bool intersect(const Ray &r, float_t &t, glm::vec4 &p_hit, uint32_t &ti);
    void get_surface_properties(const glm::vec4 &hit_point, const glm::vec4 &view_direction, const uint32_t &triangle_index,
                                    glm::vec4 &hit_normal);
    void apply_camera_transformation(glm::mat4 &t);
    void apply_transformations();
    void translate(const float_t &translation, const uint32_t &axes_of_translation);
    void rotate(const float_t &angle_of_rotation, const uint32_t &axes_of_rotation);
    void scale(const float_t &scaling_factor, const uint32_t &axes_of_scale);
};


#endif //ELUCIDO_SPHERE_H
