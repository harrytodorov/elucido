//
// Created by Haralambi Todorov on 21/06/2017.
//

#ifndef ELUCIDO_MATERIAL_H
#define ELUCIDO_MATERIAL_H


#include <glm/vec3.hpp>
#include <cmath>
#include <vector>
#include "../Utilities.h"
#include "../lights/Light.h"

class Material {
public:
    glm::vec3 c;        // color

    // default constructor
    // - default material color is white
    Material() : c(white) {}
    Material(const glm::vec3 &col) : c(col) {}
    virtual ~Material() {}

    virtual void compute_color_at_surface(const std::vector<Light *> &lights, const glm::vec4 &hit_point, const glm::vec4 &hit_normal,
                                              const glm::vec4 view_direction, glm::vec3 &color) = 0;
};


#endif //ELUCIDO_MATERIAL_H
