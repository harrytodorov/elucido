//
// Created by Haralambi Todorov on 25/05/2017.
//

#ifndef ELUCIDO_PLANE_H
#define ELUCIDO_PLANE_H


#include <glm/vec3.hpp>
#include "Object.h"

class Plane : public Object {
public:
    glm::vec3 point;        // point through which the plane passes
    glm::vec3 normal;       // normal to the plane

    Plane() :
            Object(),
            point(glm::vec3(0, 0, -300)),
            normal(glm::vec3(1, 1, 1))
    {}

    Plane(glm::vec3 &pos, glm::vec3 &nor) :
            Object(),
            point(pos),
            normal(nor)
    {}

    bool intersect(Ray &r, float_t &t);
};


#endif //ELUCIDO_PLANE_H
