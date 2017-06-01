//
// Created by Haralambi Todorov on 25/05/2017.
//

#ifndef ELUCIDO_TRIANGLE_H
#define ELUCIDO_TRIANGLE_H


#include <glm/vec4.hpp>
#include "Object.h"

class Triangle : public Object{
public:
    glm::vec4 v0, v1, v2;   // vertices of the triangle
    glm::vec4 normal;       // normal of the triangle

    Triangle() :
            Object(),
            v0(0, 0, 0, 1),
            v1(0, 0, 1, 1),
            v2(1, 0, 0, 1),
            normal(0, 1, 0, 1)
    {}

    Triangle(glm::vec4 &vec0,
             glm::vec4 &vec1,
             glm::vec4 &vec2,
             glm::vec4 &nor) :
            Object(),
            v0(vec0),
            v1(vec1),
            v2(vec2),
            normal(nor)
    {}

    bool intersect(Ray &r, float_t &t, glm::vec4 &p_hit, glm::vec4 &hit_norm);
};


#endif //ELUCIDO_TRIANGLE_H
