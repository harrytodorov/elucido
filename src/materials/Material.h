//
// Created by Haralambi Todorov on 21/06/2017.
//

#ifndef ELUCIDO_MATERIAL_H
#define ELUCIDO_MATERIAL_H


#include "../Utilities.h"
#include "../lights/Light.h"
#include <glm/vec3.hpp>
#include <cmath>
#include <vector>

class Material {
public:
    glm::vec3       c;        // color
    MaterialType    mt;

    // default constructor
    // - default material color is white
    Material(const MaterialType &mt) : c(white), mt(mt) {}
    Material(const glm::vec3 &col, const MaterialType &mt) : c(col), mt(mt) {}
    virtual ~Material() {}
};


#endif //ELUCIDO_MATERIAL_H