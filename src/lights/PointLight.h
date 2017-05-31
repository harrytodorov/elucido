//
// Created by Haralambi Todorov on 26/05/2017.
//

#ifndef ELUCIDO_POINTLIGHT_H
#define ELUCIDO_POINTLIGHT_H


#include "Light.h"

class PointLight : public Light {
public:
    glm::vec3 pos;

    PointLight(const glm::vec3 &p, const glm::vec3 &c, const float_t &i) : Light(c, i) {
        pos = p;
    }

    void illuminate(const glm::vec3 &hit_point, glm::vec3 &light_dir, glm::vec3 &light_intensity, float_t distance);
};

#endif //ELUCIDO_POINTLIGHT_H