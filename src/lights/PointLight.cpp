//
// Created by Haralambi Todorov on 26/05/2017.
//

#include <glm/geometric.hpp>
#include <glm/mat4x4.hpp>
#include "PointLight.h"

float_t norm(glm::vec4 &v) {
    return (float_t) ((v.x*v.x) + (v.y*v.y) + (v.z*v.z));
}

void PointLight::illuminate(const glm::vec4 &hit_point, glm::vec4 &light_dir, glm::vec3 &light_intensity,
                            float_t distance) {
    light_dir = hit_point - pos;
    float_t r2 = norm(light_dir);
    distance = (float_t) sqrt(r2);
    light_dir.x /= distance,
    light_dir.y /= distance,
    light_dir.z /= distance;
    light_intensity = glm::normalize((float_t) (intensity / (4.0 * M_PI * r2)) * color);
}

void PointLight::apply_transformation(glm::mat4 &t) {
    glm::vec4 res = t * pos;
    pos = res;
}
