// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include <glm/geometric.hpp>

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
                                   const glm::vec4 &light_direction) {
  return static_cast<float_t>(glm::dot(normal, light_direction));
}

//==============================================================================
glm::vec4 Renderer::reflect(const glm::vec4 &normal,
                            const glm::vec4 &to_reflect) {
  float_t la = labertian_amount(normal, to_reflect);
  return 2.f*la*normal - to_reflect;
}


