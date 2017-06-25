//
// Created by Haralambi Todorov on 25/06/2017.
//

#ifndef ELUCIDO_DIRECTIONALLIGHT_H
#define ELUCIDO_DIRECTIONALLIGHT_H


#include "Light.h"

class DirectionalLight : public Light {
public:
    glm::vec4 d;

    DirectionalLight() : Light(), d(glm::vec4(0, 0, -1, 0)) {}
    DirectionalLight(const glm::vec4 &d, const glm::vec3 &c, const float_t &i) : Light(c, i), d(d) {}
    ~DirectionalLight() {}

    void illuminate(const glm::vec4 &hit_point, glm::vec4 &light_dir, glm::vec3 &light_intensity);
    void apply_camera_transformation(glm::mat4 &t);
    void apply_transformations();
    void translate(const float_t &translation, const uint32_t &axes_of_translation);
    void rotate(const float_t &angle_of_rotation, const uint32_t &axes_of_rotation);
};


#endif //ELUCIDO_DIRECTIONALLIGHT_H
