//
// Created by Haralambi Todorov on 25/05/2017.
//

#ifndef ELUCIDO_TRIANGLE_H
#define ELUCIDO_TRIANGLE_H


#include <glm/vec3.hpp>
#include "Object.h"

class Triangle : public Object{
public:
    glm::vec3 v0, v1, v2;   // vertices of the triangle
    glm::vec3 normal;       // normal of the triangle

    Triangle() :
            Object(),
            v0(glm::vec3(0, 0, 0)),
            v1(glm::vec3(0, 0, 1)),
            v2(glm::vec3(1, 0, 0)),
            normal(0, 1, 0)
    {}

    Triangle(glm::vec3 vec0,
             glm::vec3 vec1,
             glm::vec3 vec2,
             glm::vec3 nor) :
            Object(),
            v0(vec0),
            v1(vec1),
            v2(vec2),
            normal(nor)
    {}

    bool intersect(Ray &r, float_t &t, glm::vec3 &p_hit, glm::vec3 &hit_norm);
};


#endif //ELUCIDO_TRIANGLE_H
