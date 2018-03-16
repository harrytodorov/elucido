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

  glm::vec3 cast_ray(const Ray &ray,
                     const uint32_t &depth);

  /**
   * Determine if an intersection point is in shadow from a light source.
   * @param ii:             The intersection point.
   * @param direction:      A normalized vector pointing from the surface
   *                        of the intersection point to a light source.
   * @param light_distance: The distance from the intersection point to
   *                        the light source.
   * @return:               True, if the intersection point is in shadow
   *                        from the light source, false otherwise.
   */
  bool in_shadow(const isect_info &ii,
                 const glm::vec4 &direction,
                 const float_t &light_distance);
  /**
   * Calculate the amount of Lambertian.
   *    Lambertian = dot(normal, light_direction)
   * @param normal:             The normal at the intersection point.
   * @param light_direction:    A normalized vector pointing from the surface
   *                            of the intersection point to a light source.
   * @return:                   The amount of Lambertian reflection.
   */
  inline float_t labertian_amount(const glm::vec4 &normal,
                                  const glm::vec3 &light_direction);

 protected:
  render_info                             ri;
  std::shared_ptr<AccelerationStructure>  ac;
  std::shared_ptr<AABBox>                 sbb;
  std::vector<std::shared_ptr<Object>>    objects;
  std::vector<std::shared_ptr<Light>>     lights;
};

#endif //ELUCIDO_RENDERER_H
