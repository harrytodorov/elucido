// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_ORTHOGRAPHICCAMERA_H
#define ELUCIDO_ORTHOGRAPHICCAMERA_H

#include "Camera.h"

class OrthographicCamera : public Camera {
//==============================================================================
// Constructors & destructors
//==============================================================================
 public:
  OrthographicCamera() {}
  OrthographicCamera(const float_t &z,
                     const uint32_t &_iw,
                     const uint32_t &_ih) :
      Camera(_iw, _ih),
      zf(z) {}

  ~OrthographicCamera() = default;

//==============================================================================
// Function declarations
//==============================================================================
  Ray get_ray(const uint32_t &pixel_x,
                const uint32_t &pixel_y,
                const float_t &sample_x,
                const float_t &sample_y);
  inline void set_zoom_factor(const float_t &z) { this->zf = z; }

//==============================================================================
// Data members
//==============================================================================
 protected:
  float_t zf{1.f};  // zoom factor or the size of a pixel will be used only in
                    // the orthographic camera for using it to zoom in and out
                    // of an object
};

#endif //ELUCIDO_ORTHOGRAPHICCAMERA_H
