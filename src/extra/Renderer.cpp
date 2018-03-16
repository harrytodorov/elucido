// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "Renderer.h"

//==============================================================================
glm::vec3 Renderer::cast_ray(const Ray &ray, const uint32_t &depth) {

}

//==============================================================================
bool Renderer::in_shadow(const isect_info &ii,
                         const glm::vec4 &direction,
                         const float_t &light_distance) {
  return false;
}

//==============================================================================
float_t Renderer::labertian_amount(const glm::vec4 &normal,
                                 const glm::vec3 &light_direction) {
  return 0;
}


