//
// Created by Haralambi Todorov on 11/07/2017.
//

#ifndef ELUCIDO_BBOX_H
#define ELUCIDO_BBOX_H


#include <glm/vec4.hpp>
#include "../Utilities.h"
#include "../Ray.h"

class AABBox {
public:
    glm::vec4 bounds[2] = {glm::vec4(glm::vec3(infinity), 1), glm::vec4(glm::vec3(-infinity), 1)};

    AABBox() {}
    AABBox(const glm::vec4 &min, const glm::vec4 &max) {
        bounds[0] = min;
        bounds[1] = max;
    }

    inline void reset() {
        bounds[0] = glm::vec4(glm::vec3(infinity), 1);
        bounds[1] = glm::vec4(glm::vec3(-infinity), 1);
    }
    bool intersect(const Ray &r);
    AABBox& extend_by(const glm::vec4 &p);
};


#endif //ELUCIDO_BBOX_H
