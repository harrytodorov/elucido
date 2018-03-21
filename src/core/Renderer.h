// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_RENDERER_H
#define ELUCIDO_RENDERER_H

#include <utility>
#include <vector>
#include <memory>

#include "Utilities.h"
#include "../lights/Light.h"
#include "../objects/Object.h"
#include "../accelerators/AccelerationStructure.h"

class Renderer {
 public:
  explicit Renderer(const std::shared_ptr<AccelerationStructure> _ac,
                    const std::shared_ptr<AABBox> _sbb,
                    std::vector<std::shared_ptr<Object>> _objects,
                    std::vector<std::shared_ptr<Light>> _lights) :
      ri(render_info()),
      ac(_ac.get()),
      sbb(_sbb.get()),
      objects(_objects),
      lights(_lights)
  {}

  glm::vec3 cast_ray(const Ray &ray, const uint32_t &depth);

  bool trace_ray(const Ray &r, isect_info &i);

  /**
   * Calculate the amount of Lambertian.
   *    Lambertian = dot(normal, light_direction)
   * @param normal:             The normal at the intersection point.
   * @param light_direction:    A normalized vector pointing from the surface
   *                            of the intersection point to a light source.
   * @return:                   The amount of Lambertian reflection.
   */
  float_t labertian_amount(const glm::vec4 &normal,
                           const glm::vec4 &light_direction) const;

  /**
   * Reflects the vector "to_reflect" about the normal.
   * @param normal:     Normal about which the vector would be reflected.
   * @param to_reflect: Vector to be reflected. It is assumed, that the vector
   *                    is pointing away from the surface.
   * @return:           The reflected vector pointing away from the surface.
   *                    If the vector "to_reflect" is a normal vector, the
   *                    reflected vector is also normalized by construction.
   */
  glm::vec4 reflect(const glm::vec4 &normal,
                    const glm::vec4 &to_reflect) const;

 protected:
  render_info                             ri;
  std::shared_ptr<AccelerationStructure>  ac;
  std::shared_ptr<AABBox>                 sbb;
  std::vector<std::shared_ptr<Object>>    objects;
  std::vector<std::shared_ptr<Light>>     lights;
};

#endif //ELUCIDO_RENDERER_H
