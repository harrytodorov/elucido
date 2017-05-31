//
// Created by Haralambi Todorov on 26/05/2017.
//

#include "PointLight.h"

float_t norm(glm::vec3 &v) {
    return (float_t) (v.x*v.x + v.y*v.y + v.z*v.z);
}

void
PointLight::illuminate(const glm::vec3 &hit_point, glm::vec3 &light_dir, glm::vec3 &light_intensity, float_t distance) {
    light_dir = hit_point - pos;
    float_t r2 = norm(light_dir);
    distance = (float_t) sqrt(r2);
    light_dir.x /= distance,
    light_dir.y /= distance,
    light_dir.z /= distance;
    light_intensity = (float_t) (intensity / (4.0 * M_PI * r2)) * color;
}
