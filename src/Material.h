//
// Created by Haralambi Todorov on 21/06/2017.
//

#ifndef ELUCIDO_MATERIAL_H
#define ELUCIDO_MATERIAL_H


#include <glm/vec3.hpp>
#include <cmath>
#include "Utilities.h"

class Material {
public:
    glm::vec3 c;        // material's color
    float_t   dc;       // material's diffuse constant
    float_t   sc;       // materials specular constant

    // default constructor
    // - default material color is white
    // - default diffuse constant is 0.6
    // - default specular constant is 0.2
    Material() : c(white), dc(0.6f), sc(0.2f) {}
    Material(const glm::vec3 &col, const float_t &diff, const float_t &spec) :
            c(col),
            dc(diff),
            sc(spec)
    {}
    Material(const glm::vec3 &col) : c(col), dc(0.6f), sc(0.2f) {}

};


#endif //ELUCIDO_MATERIAL_H
