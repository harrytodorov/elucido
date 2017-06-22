//
// Created by Haralambi Todorov on 19/05/2017.
//

#ifndef ELUCIDO_OBJECT_H
#define ELUCIDO_OBJECT_H

#include <glm/detail/type_vec.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "../Ray.h"
#include "../Material.h"

class Object {
public:
    Material  om;   // object's material
    glm::mat4 mt;   // model transform matrix for an object;
                    // places and transforms the object in world coordinate space

    Object() : mt(glm::mat4(1)) {}
    Object(const Material &m) : om(m), mt(glm::mat4(1)) {}
    ~Object() {}

    virtual bool intersect(Ray &r, float_t &t, glm::vec4 &p_hit, glm::vec4 &hit_norm) = 0;
    virtual void apply_transformation(glm::mat4 &t) = 0;
    virtual void translate(const float_t &translation, const uint32_t &axes_of_translation) = 0;
    virtual void rotate(const float_t &angle_of_rotation, const uint32_t &axes_of_rotation) = 0;
    virtual void scale(const float_t &scaling_factor, const uint32_t &axes_of_scale) = 0;
};


#endif //ELUCIDO_OBJECT_H
