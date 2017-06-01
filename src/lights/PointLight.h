//
// Created by Haralambi Todorov on 26/05/2017.
//

#ifndef ELUCIDO_POINTLIGHT_H
#define ELUCIDO_POINTLIGHT_H


#include "Light.h"

class PointLight : public Light {
public:
    glm::vec4 pos;

    PointLight(const glm::vec4 &p, const glm::vec3 &c, const float_t &i) : Light(c, i) {
        pos = p;
    }

    void illuminate(const glm::vec4 &hit_point, glm::vec4 &light_dir, glm::vec3 &light_intensity, float_t distance);
    void apply_transformation(glm::mat4 &t);
};

#endif //ELUCIDO_POINTLIGHT_H