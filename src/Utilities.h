//
// Created by Haralambi Todorov on 25/05/2017.
//

#ifndef ELUCIDO_UTILITIES_H
#define ELUCIDO_UTILITIES_H

#include "glm/gtc/epsilon.hpp"

const float_t kEpsilon = glm::epsilon<float_t>();   // epsilon value; used to deal with some edge cases

const float_t kd = 0.8;                             // Phong model diffuse weight
const float_t ks = 0.2;                             // Phong model specular weight
const float_t ka = 0.2;                             // Phong model ambient constant
const float_t n = 10;                               // Phong specular exponent

#endif //ELUCIDO_UTILITIES_H