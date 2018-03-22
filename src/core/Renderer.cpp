// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include <glm/geometric.hpp>

#include "Renderer.h"

//==============================================================================
glm::vec3 Renderer::cast_ray(const Ray &ray, const uint32_t &depth) {
  // Check if the bottom of the ray tree is reached.
  if (depth > max_depth) return bgc;

  // Hit radiance.
  glm::vec3 hr(bgc);

  // Structure to hold the intersection information.
  isect_info i;

  // Trace a ray through the scene.
  if (!trace_ray(ray, i)) return hr;

  // If there is an intersection, reset the radiance to 0.
  hr = glm::vec3(0);
  
  // Material of the intersected object.
  MaterialType mt = i.ho->material().mt;

  // Evaluate Phong.
  if (mt == phong) return evaluate_phong(i, ray.dir());


}

//==============================================================================
glm::vec3 Renderer::evaluate_phong(const isect_info &i,
                                   const glm::vec4 &ray_direction) {
  material m = i.ho->material();
  glm::vec3 diffuse{0.f}, specular{0.f};
  float_t labertian{0.f};

  for (auto const &light : lights) {
    glm::vec4 light_direction = light->get_direction(i.ip);
    float_t   light_distance  = light->get_distance(i.ip);
    glm::vec3 light_intensity = (light->color() *
                                light->get_intensity(light_distance));

    // Check if the intersection point is in shadow for the light
    // source.
    isect_info si;

    Ray shadow_ray;
    shadow_ray.rt = shadow;
    shadow_ray.set_orig(i.ip + bias * i.ipn);
    shadow_ray.set_dir(light_direction);

    auto in_shadow = trace_ray(shadow_ray, si);
    if (in_shadow && si.tn < light_distance) continue;

    labertian = labertian_amount(i.ipn, light_direction);

    // Increment diffuse component.
    diffuse += light_intensity * glm::clamp(labertian, 0.f, 1.f);

    // Calculate specular component only, if the intersection point normal
    // is oriented towards the light source.
    if (labertian > 0.f) {
      auto lightDir_reflection  = reflect(i.ipn, light_direction);
      auto lr_vd_dot            = glm::dot(lightDir_reflection, -ray_direction);
      auto specular_term        = glm::pow(lr_vd_dot, m.se);

      // Increment specular component.
      specular += light_intensity * specular_term;
    }
  }

  // Combine ambient, diffuse and specular component.
  return m.ac * m.c + m.dc * m.c * diffuse + m.sc * specular;
}

//==============================================================================
float_t Renderer::labertian_amount(const glm::vec4 &normal,
                                   const glm::vec4 &light_direction) const {
  return static_cast<float_t>(glm::dot(normal, light_direction));
}

//==============================================================================
glm::vec4 Renderer::reflect(const glm::vec4 &normal,
                            const glm::vec4 &to_reflect) const {
  float_t la = labertian_amount(normal, to_reflect);
  return 2.f*la*normal - to_reflect;
}

//==============================================================================
bool Renderer::trace_ray(const Ray &r, isect_info &i) {

  // Increment primary rays.
  if (r.rt == primary) __sync_fetch_and_add(&ri.npr, 1);

  // Increment shadow rays.
  if (r.rt == shadow) __sync_fetch_and_add(&ri.nsr, 1);

  // Increment reflection rays.
  if (r.rt == reflection) __sync_fetch_and_add(&ri.nrr, 1);

  // Increment refraction rays.
  if (r.rt == refraction) __sync_fetch_and_add(&ri.nrrr, 1);

  // Check if the ray intersects within the scene bounds.
  if (sbb != nullptr && !sbb->intersect(r)) return false;

  // Leave the acceleration structure to find the intersection point.
  // TODO: increment intersections count (primitive and object)
  if (ac != nullptr && r.rt != shadow) return ac->intersect(r, i);

  // Iterate through objects and find the closest intersection.
  for (const auto &object : objects) {
    // Intersection information for the current object.
    isect_info co;

    // Increment ray-object tests, bounding box.
    __sync_fetch_and_add(&ri.nrpt, 1);

    // First intersect with object's bounding box.
    // One only intersect triangulated meshes with their bounding boxes,
    // because tests for spheres and triangles are already cheap enough.
    if (object->object_type() == triangle_mesh &&
        !object->bounding_box().intersect(r))
      continue;

    // The number of ray-primitive intersections for triangulated mesh
    // is equal to the number of triangles in the mesh.
    if (object->object_type() == triangle_mesh)
      __sync_fetch_and_add(&ri.nrpt,
                           std::static_pointer_cast<TriangleMesh>(object)->nt);
    else
      __sync_fetch_and_add(&ri.nrpt, 1);

    // If there is an intersection with the object's bounding box,
    // try to intersect with the object itself.
    if (!object->intersect(r, co) || co.tn > i.tn) continue;

    i = co;
    i.ho = object;

    // increment the number of ray-object intersections
    __sync_fetch_and_add(&ri.nroi, 1);
  }

  return (i.ho != nullptr);
}