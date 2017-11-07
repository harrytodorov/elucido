// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_PERSPECTIVECAMERA_H
#define ELUCIDO_PERSPECTIVECAMERA_H

#include "Camera.h"

class PerspectiveCamera : public Camera {

//=============================================================================
// Data members
//=============================================================================
  float_t fov{90.f};     // angle of view / field of view

//=============================================================================
// Constructors & destructors
//=============================================================================
 public:
  PerspectiveCamera() = default;

//=============================================================================
  explicit PerspectiveCamera(const float_t &f) : fov(f) {}

//=============================================================================
// Function declarations, inline functions
//=============================================================================
  render_info render_scene(const std::vector<Object *> &objects,
                           const std::vector<Light *> &lights,
                           ImagePlane &ip);
  inline void set_fov(const float_t &f) {
    this->fov = f;
  }
};

#endif //ELUCIDO_PERSPECTIVECAMERA_H
