//
// Created by Haralambi Todorov on 19/05/2017.
//

#ifndef ELUCIDO_OBJECT_H
#define ELUCIDO_OBJECT_H


#include <glm/detail/type_vec.hpp>
#include <glm/vec3.hpp>
#include <cmath>
#include "../Ray.h"

class Object {
public:
    glm::vec3 color;

    Object() : color(glm::vec3(255)) {}
    Object(Object &o) : color(o.color) {}
    ~Object() {}

    virtual bool intersect(Ray &r, float_t &t, glm::vec4 &p_hit, glm::vec4 &hit_norm) = 0;
};


#endif //ELUCIDO_OBJECT_H
