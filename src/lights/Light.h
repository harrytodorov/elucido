//
// Created by Haralambi Todorov on 26/05/2017.
//

#ifndef ELUCIDO_LIGHT_H
#define ELUCIDO_LIGHT_H


#include <glm/vec3.hpp>
#include <cmath>

class Light {
public:
    glm::vec3 color;
    float_t   intensity;

    // default light color is white (255, 255, 255)
    // default light intensity is 100
    Light(const glm::vec3 &c, const float_t &i) : color(c), intensity(i) {}
    Light() : color(glm::vec3(255)), intensity(100.0) {}

    virtual void illuminate(const glm::vec4 &hit_point, glm::vec4 &light_dir, glm::vec3 &light_intensity,
                            float_t distance) = 0;
    virtual void apply_transformation(glm::mat4 &t);
};


#endif //ELUCIDO_LIGHT_H
