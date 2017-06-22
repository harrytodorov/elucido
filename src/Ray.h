//
// Created by Haralambi Todorov on 18/05/2017.
//

#ifndef ELUCIDO_RAY_H
#define ELUCIDO_RAY_H

#include "glm/vec4.hpp"
#include "Utilities.h"

class Ray {

public:
    glm::vec4 orig;    // the origin point of the ray
    glm::vec4 dir;     // the direction the ray is pointing to
    RayType   rt;      // the type of the casted ray

    // default Ray constructor
    Ray():
            orig(0, 0, 0, 1),
            dir(0, 0, -1, 0),
            rt(primary)
    {}

    Ray(const glm::vec4 &o, const glm::vec4 &d) {
        orig    = o;
        dir     = d;
        rt      = primary;
    }

    // destructor
    ~Ray() {}
};


#endif //ELUCIDO_RAY_H
