// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_CAMERA_H
#define ELUCIDO_CAMERA_H

#include "../core/Utilities.h"
#include "../core/ImagePlane.h"
#include "../lights/Light.h"
#include "../accelerators/AccelerationStructure.h"
#include "../accelerators/Grid.h"

class Camera {
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

  // TODO: Following functions should be removed from the Camera.
  // Goes to Scene as render image.
  virtual render_info render_scene(const std::vector<std::shared_ptr<Object>> &objects,
                                   const std::vector<std::shared_ptr<Light>> &lights,
                                   ImagePlane &ip) = 0;

  // Goes to Renderer.
  glm::vec4 refract(const glm::vec4 &incident_direction,
                    const glm::vec4 &surface_normal,
                    const float_t &ior);
  // Goes to Renderer.
  void compute_fresnel(const glm::vec4 &incident_direction,
                       const glm::vec4 &surface_normal,
                       const float_t &ior,
                       float_t &reflectance);

 protected:
  glm::vec4 eye;
  glm::vec4 lookat;
  uint32_t  iw;         // Image's height.
  uint32_t  ih;         // Image's width.
  float_t   ar;         // Aspect ratio [ width / height ]
  glm::mat4 vm;
};
#endif //ELUCIDO_CAMERA_H