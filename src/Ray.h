//
// Created by Haralambi Todorov on 18/05/2017.
//

#ifndef ELUCIDO_RAY_H
#define ELUCIDO_RAY_H

#include "glm/vec3.hpp"

class Ray {

public:
    glm::vec3 orig;    // the origin point of the ray
    glm::vec3 dir;     // the direction the ray is pointing to

    // default Ray constructor
    Ray():
            orig(glm::dvec3(0.0, 0.0, 0.0)),
            dir(glm::dvec3(0.0, 0.0, -1.0))
    {}

    Ray(const glm::dvec3 &o, const glm::dvec3 &d) {
        orig    = o;
        dir     = d;
    }

    // destructor
    ~Ray() {}
};


#endif //ELUCIDO_RAY_H
