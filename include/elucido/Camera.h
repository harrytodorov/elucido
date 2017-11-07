// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_CAMERA_H
#define ELUCIDO_CAMERA_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>
#include <vector>
#include <iostream>
#include <random>
#include "Object.h"
#include "ImagePlane.h"
#include "Light.h"
#include "Utilities.h"

class Camera {

//=============================================================================
// Data members
//=============================================================================
 protected:
  glm::vec4 eye;        // eye / camera position
  glm::vec4 lookat;     // the point at which the camera looks
  AABBox scene_bb;   // bounding box for the scene
 private:
  glm::mat4 ctm;        // camera's transformation matrix
  glm::mat4
      tictm;      // the transpose of the inverse of the camera's transformation matrix;

//=============================================================================
// Constructors & destructors
//=============================================================================
// - default camera position is at (0, 0, 0) in world space
// - default camera direction is the negative z-axis (0, 0, -1)
 public:
  Camera() :
      eye(0, 0, 0, 1),
      lookat(0, 0, -1, 1),
      ctm(1),
      tictm(1),
      scene_bb() {}

//=============================================================================
  Camera(const glm::vec4 &p, const glm::vec4 &d) :
      eye(p),
      lookat(d),
      ctm(1),
      tictm(1),
      scene_bb() {}

//=============================================================================
  ~Camera() = default;


//=============================================================================
// Function declarations, inline functions
//=============================================================================
  void translate(const float_t &translation,
                 const uint32_t &axes_of_translation);
  void rotate(float_t rot_angle, uint32_t axes_of_rotation);
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
  virtual render_info render_scene(const std::vector<Object *> &objects,
                                   const std::vector<Light *> &lights,
                                   ImagePlane &ip) = 0;
  inline glm::vec4 reflect(const glm::vec4 &incident_direction,
                           const glm::vec4 &surface_normal) {
    return incident_direction
        - 2.f * glm::dot(incident_direction, surface_normal) * surface_normal;
  };
 protected:
  void apply_inverse_view_transform(const std::vector<Object *> &objects,
                                    const std::vector<Light *> &lights);
  void reverse_inverse_view_transform(const std::vector<Object *> &objects,
                                      const std::vector<Light *> &lights);
  void extend_scene_bb(const std::vector<Object *> &objects);
 private:
  inline glm::mat4 inverse_ctm() {
    // return the inverse of the camera's transformation matrix
    // needed to apply it to objects and light sources
    return glm::inverse(ctm);
  };
  inline glm::mat4 inverse_tictm() {
    // return the inverse of the transpose of the inverse of the camera's transformation matrix
    return glm::inverse(glm::transpose(tictm));
  };
};
#endif //ELUCIDO_CAMERA_H