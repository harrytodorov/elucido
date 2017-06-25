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
    glm::vec3 c;        // color
    float_t   ac;       // ambient constant
    float_t   dc;       // diffuse constant
    float_t   sc;       // specular constant
    float_t   se;       // specular exponent

    // default constructor
    // - default material color is white
    // - default ambient constant is 0.2
    // - default diffuse constant is 0.6
    // - default specular constant is 0.2
    // - default specular exponent is 10
    Material() : c(white), ac(0.2f), dc(0.6f), sc(0.2f), se(10.f) {}
    Material(const glm::vec3 &col, const float_t &diff, const float_t &spec) :
            c(col),
            dc(diff),
            sc(spec)
    {}
    Material(const glm::vec3 &col) : c(col), dc(0.6f), sc(0.2f) {}
    ~Material() {}

};


#endif //ELUCIDO_MATERIAL_H
