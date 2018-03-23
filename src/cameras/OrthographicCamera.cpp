// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "OrthographicCamera.h"

Ray OrthographicCamera::get_ray(const uint32_t &pixel_x,
                                const uint32_t &pixel_y,
                                const float_t &sample_x,
                                const float_t &sample_y) {
  auto x = (2.f * ((pixel_x + sample_x) / iw) - 1.f) * ar * zf;
  auto y = (1.f - 2.f * ((pixel_y + sample_y) / ih)) * zf;
  auto o = glm::vec4(x, y, eye.z, 1.f);

  Ray r;
  r.rt = primary;
  r.set_orig(o);
  r.set_dir(glm::normalize(lookat - eye));

  return r;
}
