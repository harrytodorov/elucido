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
    RayType   rt;       // the type of the casted ray

    // default Ray constructor
    Ray() : o(0, 0, 0, 1), d(0, 0, -1, 0), rt(primary) {
        id = 1.f / d;
        s[0] = (uint32_t) (id.x < 0);
        s[1] = (uint32_t) (id.y < 0);
        s[2] = (uint32_t) (id.z < 0);
    }
    Ray(const glm::vec4 &o, const glm::vec4 &d) : o(o), d(d), rt(primary) {
        id = 1.f / d;
        s[0] = (uint32_t) (id.x < 0);
        s[1] = (uint32_t) (id.y < 0);
        s[2] = (uint32_t) (id.z < 0);
    }
    ~Ray() {}

    bool trace(const std::vector<Object *> &objects, isect_info &ii, render_info &ri) const;
    inline glm::vec4 orig() const {
        return this->o;
}              // get origin
    inline void set_orig(const glm::vec4 &_o) {
        this->o = _o;
    }  // set origin
    inline glm::vec4 dir() const {
        return this->d;
    }               // get direction
    inline void set_dir(const glm::vec4 &_d) {
        this->d = _d;
        id = 1.f / d;
        s[0] = (uint32_t) (id.x < 0);
        s[1] = (uint32_t) (id.y < 0);
        s[2] = (uint32_t) (id.z < 0);
    }   // set direction
    inline const uint32_t* get_sign() const {
        return this->s;
    }    // get signs of the ray direction components
    inline glm::vec4 get_inv_dir() const {
        return this->id;
    }       // get the inverse of the direction
};


#endif //ELUCIDO_RAY_H
