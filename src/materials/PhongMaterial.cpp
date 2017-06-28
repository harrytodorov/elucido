//
// Created by Haralambi Todorov on 27/06/2017.
//

#include "PhongMaterial.h"

void PhongMaterial::compute_color_at_surface(const std::vector<Light *> &lights, const glm::vec4 &hit_point,
                                             const glm::vec4 &hit_normal, const glm::vec4 view_direction,
                                             glm::vec3 &color) {
    // set the hit color to black before adding the ambient, defuse and specular components
    // in case the default background color is not black
    color = black;

    // holders for diffuse & specular values;
    glm::vec3 diffuse(0), specular(0);
    float_t lambertian_refl(0);

    // iterate through all light sources and calculate specular and defuse components
    for (auto& light : lights) {
        glm::vec4 light_direction(0);
        glm::vec3 light_intensity(0);
        light->illuminate(hit_point, light_direction, light_intensity);

        // dot product based on Lambert's cosine law for Lambertian reflectance;
        lambertian_refl = glm::dot(hit_normal, light_direction);

        // calculate diffuse component
        diffuse += c * light_intensity * std::max(0.f, lambertian_refl);

        // calculate specular component
        glm::vec4 light_reflection = glm::normalize(2.f * lambertian_refl * hit_normal - light_direction);
        specular += light_intensity * std::powf(std::max(0.f, glm::dot(light_reflection, view_direction)), se);
    }
    // add ambient, diffuse and specular to the the hit color
    color += ac * c + dc * diffuse + sc * specular;
}
