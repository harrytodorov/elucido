//
// Created by Haralambi Todorov on 18/05/2017.
//

#ifndef ELUCIDO_RAY_H
#define ELUCIDO_RAY_H

#include "glm/vec4.hpp"

#include <vector>

#include "objects/Object.h"
#include "Utilities.h"

class Ray {
    glm::vec4 o;        // the origin point of the ray
    glm::vec4 d;        // the direction of the ray
    glm::vec4 id;       // the inverse of the ray direction; needed to optimize AABB calculation
    uint32_t  s[3];     // the sign of the ray direction; needed to optimize AABB calculation
public:
    RayType   rt{primary};       // the type of the casted ray

    // default Ray constructor
    Ray() : o(0, 0, 0, 1), d(0, 0, -1, 0) {
        id = 1.f / d;
        s[0] = (uint32_t) (id.x < 0);
        s[1] = (uint32_t) (id.y < 0);
        s[2] = (uint32_t) (id.z < 0);
    }
    Ray(const glm::vec4 &o, const glm::vec4 &d) : o(o), d(d) {
        id = 1.f / d;
        s[0] = (uint32_t) (id.x < 0);
        s[1] = (uint32_t) (id.y < 0);
        s[2] = (uint32_t) (id.z < 0);
    }
    ~Ray() = default;

    bool trace(const std::vector<Object *> &objects, isect_info &ii, render_info &ri) const;

    // get origin
    inline glm::vec4 orig() const { return this->o; }

    // get direction
    inline glm::vec4 dir() const { return this->d; }

    // get signs of the ray direction components
    inline const uint32_t* sign() const { return this->s; }

    // get the inverse of the direction
    inline glm::vec4 inv_dir() const { return this->id; }

    // set origin
    inline void set_orig(const glm::vec4 &_o) { this->o = _o; }
    
    // set direction
    void set_dir(const glm::vec4 &_d) {
        this->d = _d;
        id = 1.f / d;
        s[0] = (uint32_t) (id.x < 0);
        s[1] = (uint32_t) (id.y < 0);
        s[2] = (uint32_t) (id.z < 0);
    }
};


#endif //ELUCIDO_RAY_H
