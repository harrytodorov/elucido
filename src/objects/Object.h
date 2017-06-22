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

    Object() {}
    Object(const Material &m) : om(m) {}
    ~Object() {}

    virtual bool intersect(Ray &r, float_t &t, glm::vec4 &p_hit, glm::vec4 &hit_norm) = 0;
    virtual void apply_transformation(glm::mat4 &t) = 0;
};


#endif //ELUCIDO_OBJECT_H
