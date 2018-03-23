// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_PERSPECTIVECAMERA_H
#define ELUCIDO_PERSPECTIVECAMERA_H

#include "Camera.h"

class PerspectiveCamera : public Camera {
 public:
  PerspectiveCamera() { sf = glm::tan(glm::radians(fov * 0.5f)); }
  PerspectiveCamera(const float_t &f) : fov(f) {
    sf = glm::tan(glm::radians(fov * 0.5f));
  }
  PerspectiveCamera(const float_t &f,
                    const uint32_t &_iw,
                    const uint32_t &_ih) :
      Camera(_iw, _ih),
      fov(f)
      {
        sf = glm::tan(glm::radians(fov * 0.5f));
      }
  ~PerspectiveCamera() = default;

  Ray get_ray(const uint32_t &pixel_x,
                const uint32_t &pixel_y,
                const float_t &sample_x,
                const float_t &sample_y);
  inline void set_fov(const float_t &f) { this->fov = f; }

 protected:
  float_t fov{90.f};  // Field of view.
  float_t sf;         // Scale factor.
};

#endif //ELUCIDO_PERSPECTIVECAMERA_H
