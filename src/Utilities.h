//
// Created by Haralambi Todorov on 25/05/2017.
//

#ifndef ELUCIDO_UTILITIES_H
#define ELUCIDO_UTILITIES_H

#include "glm/gtc/epsilon.hpp"

const float_t kEpsilon = glm::epsilon<float_t>();               // epsilon value; used to deal with some edge cases
const float_t infinity = std::numeric_limits<float_t>::max();   // infinity value

const uint32_t X    = 0;
const uint32_t Y    = 1;
const uint32_t Z    = 2;
const uint32_t XY   = 3;
const uint32_t XZ   = 4;
const uint32_t YZ   = 5;
const uint32_t XYZ  = 6;

const glm::vec3 red(1.f, 0, 0);                 // red color
const glm::vec3 green(0, 1.f, 0);               // green color
const glm::vec3 blue(0, 0, 1.f);                // blue color
const glm::vec3 white(1);                       // white color
const glm::vec3 black(0);                       // black color
const glm::vec3 sienna(0.627f, 0.321f, 0.176f); // sienna color

enum RayType: uint8_t {
    primary,

};



#endif //ELUCIDO_UTILITIES_H