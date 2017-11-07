// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_ORTHOGRAPHICCAMERA_H
#define ELUCIDO_ORTHOGRAPHICCAMERA_H

#include "Camera.h"

class OrthographicCamera : public Camera {

//=============================================================================
// Data members
//=============================================================================
  float_t zf{1.f};  // zoom factor or the size of a pixel will be used only in
                    // the orthographic camera for using it to zoom in and out
                    // of an object

//=============================================================================
// Constructors & destructors
//=============================================================================
 public:
  OrthographicCamera() = default;

//=============================================================================
  explicit OrthographicCamera(const float_t &z) : zf(z) {}

//=============================================================================
// Function declarations, inline functions
//=============================================================================
  render_info render_scene(const std::vector<Object *> &objects,
                           const std::vector<Light *> &lights,
                           ImagePlane &ip);
  inline void set_zoom_factor(const float_t &z) {
    this->zf = z;
  }
};

#endif //ELUCIDO_ORTHOGRAPHICCAMERA_H
