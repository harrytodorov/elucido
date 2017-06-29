//
// Created by Haralambi Todorov on 26/05/2017.
//

#ifndef ELUCIDO_POINTLIGHT_H
#define ELUCIDO_POINTLIGHT_H


#include "Light.h"

class PointLight : public Light {
public:
    glm::vec4 p;

    PointLight() : Light(), p(glm::vec4(0, 0, 0, 1)) {}
    PointLight(const glm::vec4 &p, const glm::vec3 &c, const float_t &i) : Light(c, i), p(p) {}
    ~PointLight() {}

    void illuminate(const glm::vec4 &hit_point, glm::vec4 &light_dir, glm::vec3 &light_intensity, float_t &distance);
    void apply_camera_transformation(glm::mat4 &t);
    void apply_transformations();
    void translate(const float_t &translation, const uint32_t &axes_of_translation);
    void rotate(const float_t &angle_of_rotation, const uint32_t &axes_of_rotation);
};

#endif //ELUCIDO_POINTLIGHT_H