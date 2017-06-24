//
// Created by Haralambi Todorov on 25/05/2017.
//

#ifndef ELUCIDO_UTILITIES_H
#define ELUCIDO_UTILITIES_H

#include "glm/gtc/epsilon.hpp"

const float_t kEpsilon = glm::epsilon<float_t>();               // epsilon value; used to deal with some edge cases
const float_t infinity = std::numeric_limits<float_t>::max();   // infinity value

const float_t kd = 0.8;     // Phong model diffuse weight
const float_t ks = 0.2;     // Phong model specular weight
const float_t ka = 0.2f;    // Phong model ambient constant
const float_t n  = 10;      // Phong specular exponent

const uint32_t X    = 0;
const uint32_t Y    = 1;
const uint32_t Z    = 2;
const uint32_t XY   = 3;
const uint32_t XZ   = 4;
const uint32_t YZ   = 5;
const uint32_t XYZ  = 6;

const glm::vec3 red(255, 0, 0);         // red color
const glm::vec3 green(0, 255, 0);       // green color
const glm::vec3 blue(0, 0, 255);        // blue color
const glm::vec3 white(255);             // white color
const glm::vec3 black(0);               // black color
const glm::vec3 sienna(160,82,45);      // sienna color

enum RayType: uint8_t {
    primary,

};

#endif //ELUCIDO_UTILITIES_H