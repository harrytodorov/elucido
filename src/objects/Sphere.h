//
// Created by Haralambi Todorov on 19/05/2017.
//

#ifndef ELUCIDO_SPHERE_H
#define ELUCIDO_SPHERE_H

#include <glm/geometric.hpp>
#include "Object.h"

class Sphere : public Object {
public:
    Sphere() : Object(), r(1.0), c(0, 0, 0, 1) { r2 = 1.f; }
    Sphere(const glm::dvec4 &c, const float_t &r) : Object(), r(r), c(c) { r2 = powf(r, 2.f); }
    ~Sphere();

    virtual void set_radius(const float_t &r);
    void set_center_p(const glm::vec4 &p);
    bool intersect(const Ray &r, float_t &t, glm::vec4 &p_hit, glm::vec4 &hit_norm);
    void apply_camera_transformation(glm::mat4 &t);
    void translate(const float_t &translation, const uint32_t &axes_of_translation);
    void rotate(const float_t &angle_of_rotation, const uint32_t &axes_of_rotation);
    void scale(const float_t &scaling_factor, const uint32_t &axes_of_scale);

private:
    float_t     r;  // sphere's radius
    float_t     r2; // the squared sphere's radius; needed to compute the ray-sphere intersecion
    // computed once, by constructing the sphere
    // TODO: think of the case when the sphere is scaled and take care of it's squared radius
    glm::vec4   c;  // sphere's center
};


#endif //ELUCIDO_SPHERE_H
