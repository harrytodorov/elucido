// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_CAMERA_H
#define ELUCIDO_CAMERA_H

#include <cstdint>
#include <glm/mat4x4.hpp>

#include "../core/Ray.h"
#include "../lights/Light.h"
#include "../objects/Object.h"

class Camera {
//==============================================================================
// Constructors & destructors
//==============================================================================
 public:
  Camera() :
      eye(0, 0, 0, 1),
      lookat(0, 0, -1, 1),
      vm(1),
      iw(1),
      ih(1) {
    calculate_ar();
  }
  Camera(const uint32_t &iw, const uint32_t &ih) :
      eye(0, 0, 0, 1),
      lookat(0, 0, -1, 1),
      vm(1),
      iw(iw),
      ih(ih) {
    calculate_ar();
  }
  virtual ~Camera() = default;

//==============================================================================
// Function declarations
//==============================================================================
  void translate(const float_t &translation,
                 const Axis &translation_axis);
  void rotate(const float_t &rot_angle, const Axis &rotation_axis);
  void apply_inverse_view_transform(const std::vector<std::shared_ptr<Object>> &objects,
                                    const std::vector<std::shared_ptr<Light>> &lights);
  void reverse_inverse_view_transform(const std::vector<std::shared_ptr<Object>> &objects,
                                      const std::vector<std::shared_ptr<Light>> &lights);
  virtual Ray get_ray(const uint32_t &pixel_x,
                      const uint32_t &pixel_y,
                      const float_t &sample_x,
                      const float_t &sample_y) = 0;

  inline void set_image_width (const uint32_t &_iw) {
    this->iw = _iw;
    calculate_ar();
  }
  inline void set_image_height(const uint32_t &_ih) {
    this->ih = _ih;
    calculate_ar();
  }
  inline void calculate_ar() { ar = (iw * 1.f) / ih; }

//==============================================================================
// Data members
//==============================================================================
 protected:
  glm::vec4 eye;
  glm::vec4 lookat;
  uint32_t  iw;     // Image's height.
  uint32_t  ih;     // Image's width.
  float_t   ar;     // Aspect ratio [ width / height ]
  glm::mat4 vm;     // View matrix.
};
#endif //ELUCIDO_CAMERA_H