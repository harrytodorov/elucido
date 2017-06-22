//
// Created by Haralambi Todorov on 25/05/2017.
//

#ifndef ELUCIDO_TRIANGLE_H
#define ELUCIDO_TRIANGLE_H

#define GLM_SWIZZLE_XYZW

#include "Object.h"

class Triangle : public Object{
public:
    glm::vec4 v0, v1, v2;   // vertices of the triangle
    glm::vec4 normal;       // normal of the triangle

    // Vertices have to be defined in counterclockwise direction, starting from vertex v0 going to v2
    // otherwise strange artefacts could occur

    Triangle() :
            Object(),
            v0(-1, -1, 0, 1),
            v1( 1, -1, 0, 1),
            v2( 0,  1, 0, 1),
            normal(0, 0, 1, 1)
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
    Triangle(glm::vec4 &vec0,
             glm::vec4 &vec1,
             glm::vec4 &vec2) {
        v0 = vec0;
        v1 = vec1;
        v2 = vec2;
        normal = glm::normalize(glm::vec4(glm::cross(glm::vec3(v1)-glm::vec3(v0), glm::vec3(v2)-glm::vec3(v0)), 0));
    }

    bool intersect(Ray &r, float_t &t, glm::vec4 &p_hit, glm::vec4 &hit_norm);
    void apply_transformation(glm::mat4 &t);
};


#endif //ELUCIDO_TRIANGLE_H
