//
// Created by Haralambi Todorov on 19/05/2017.
//

#ifndef ELUCIDO_SPHERE_H
#define ELUCIDO_SPHERE_H

#include <glm/geometric.hpp>
#include "Object.h"

class Sphere : public Object {
public:
    float_t radius;
    glm::vec4 center;

    Sphere() :
            Object(),
            radius(1.0),
            center(0, 0, 0, 1)
    {}

    Sphere(const glm::dvec4 &c, const float_t &r) : Object(), radius(r), center(c) {}

    bool intersect(Ray &r, float_t &t, glm::vec4 &p_hit, glm::vec4 &hit_norm);
    void apply_camera_inverse(glm::mat4 &t);
    void translate(const float_t &translation, const uint32_t &axes_of_translation);
    void rotate(const float_t &angle_of_rotation, const uint32_t &axes_of_rotation);
    void scale(const float_t &scaling_factor, const uint32_t &axes_of_scale);
};


#endif //ELUCIDO_SPHERE_H
