//
// Created by Haralambi Todorov on 26/05/2017.
//

#ifndef ELUCIDO_POINTLIGHT_H
#define ELUCIDO_POINTLIGHT_H


#include "Light.h"

class PointLight : public Light {
public:
    glm::vec4 p{glm::vec4(0, 0, 0, 1)};

    PointLight() = default;
    PointLight(const glm::vec4 &p, const float_t &i) : Light(i), p(p) {}
    PointLight(const glm::vec4 &p, const glm::vec3 &c, const float_t &i) : Light(c, i), p(p) {}
    ~PointLight() = default;

    void illuminate(const glm::vec4 &hit_point, glm::vec4 &light_dir, glm::vec3 &light_intensity, float_t &distance);
    void apply_camera_transformation(const glm::mat4 &ictm, const glm::mat4 &itctm);
    void apply_transformations();
    void translate(const float_t &translation, const uint32_t &axes_of_translation);
    void rotate(const float_t &angle_of_rotation, const uint32_t &axes_of_rotation);
};

#endif //ELUCIDO_POINTLIGHT_H