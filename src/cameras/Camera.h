// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_CAMERA_H
#define ELUCIDO_CAMERA_H

#include <vector>
#include <iostream>
#include <random>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

#include "../extra/Utilities.h"
#include "../objects/Object.h"
#include "../extra/ImagePlane.h"
#include "../lights/Light.h"
#include "../accelerators/AccelerationStructure.h"
#include "../accelerators/Grid.h"

class Camera {

//=============================================================================
// Data members
//=============================================================================
 protected:
  glm::vec4 eye;        // eye / camera position
  glm::vec4 lookat;     // the point at which the camera looks
  AABBox scene_bb;      // bounding box for the scene
  bool use_as;          // Use an accelerators structure.
  float_t grid_alpha;   // Alpha parameter of the grid accelerators structure.
 private:
  glm::mat4 vm;         // Camera's view matrix.

//=============================================================================
// Constructors & destructors
//=============================================================================
// - default camera position is at (0, 0, 0) in world space
// - default camera direction is the negative z-axis (0, 0, -1)
 public:
  Camera() :
      eye(0, 0, 0, 1),
      lookat(0, 0, -1, 1),
      vm(1),
      use_as(true),
      scene_bb() {}

//=============================================================================
  Camera(const glm::vec4 &p, const glm::vec4 &d) :
      eye(p),
      lookat(d),
      vm(1),
      use_as(true),
      scene_bb() {}

//=============================================================================
Camera(const glm::vec4 &p, const glm::vec4 &d, const bool &as) :
    eye(p),
    lookat(d),
    vm(1),
    use_as(as),
    scene_bb() {}

  virtual
  ~Camera() = default;

//=============================================================================
// Function declarations, inline functions
//=============================================================================
  void translate(const float_t &translation,
                 const Axis &translation_axis);
  void rotate(const float_t &rot_angle, const Axis &rotation_axis);
  void apply_inverse_view_transform(const std::vector<Object *> &objects,
                                    const std::vector<Light *> &lights);
  void reverse_inverse_view_transform(const std::vector<Object *> &objects,
                                      const std::vector<Light *> &lights);
  void extend_scene_bb(const std::vector<Object *> &objects);
  virtual Ray get_ray(const uint32_t &pixel_x,
                        const uint32_t &pixel_y,
                        const float_t &sample_x,
                        const float_t &sample_y,
                        const uint32_t &width,
                        const uint32_t &height) = 0;

  virtual render_info render_scene(const std::vector<Object *> &objects,
                                   const std::vector<Light *> &lights,
                                   ImagePlane &ip) = 0;

  glm::vec4 refract(const glm::vec4 &incident_direction,
                    const glm::vec4 &surface_normal,
                    const float_t &ior);
  void compute_fresnel(const glm::vec4 &incident_direction,
                       const glm::vec4 &surface_normal,
                       const float_t &ior,
                       float_t &reflectance);
  glm::vec3 cast_ray(const Ray &ray,
                     const std::vector<Light *> &lights,
                     const std::vector<Object *> &objects,
                     const uint32_t &depth,
                     render_info &ri);
  glm::vec3 cast_ray(const Ray &ray,
                     const std::vector<Light *> &lights,
                     const std::vector<Object *> &objects,
                     const uint32_t &depth,
                     const AccelerationStructure *structure,
                     render_info &ri);

  inline void use_acceleration(const bool &as, const float_t &ga = 3) {
    this->use_as = as;
    this->grid_alpha = ga;
  }

  inline glm::vec4 reflect(const glm::vec4 &incident_direction,
                           const glm::vec4 &surface_normal) {
    return incident_direction
        - 2.f * glm::dot(incident_direction, surface_normal) * surface_normal;
  };
};
#endif //ELUCIDO_CAMERA_H