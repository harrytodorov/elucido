// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "PerspectiveCamera.h"

Ray PerspectiveCamera::get_ray(const uint32_t &pixel_x,
                               const uint32_t &pixel_y,
                               const float_t &sample_x,
                               const float_t &sample_y) {
  auto x  = (2.f * ((pixel_x + sample_x) / iw) - 1.f) * ar * sf;
  auto y  = (1.f - 2.f * ((pixel_y + sample_y) / ih)) * sf;
  auto cp = glm::vec4(x, y, lookat.z, 1.f);
  auto d  = glm::normalize(cp - eye);

  Ray r;
  r.rt = primary;
  r.set_orig(eye);
  r.set_dir(d);

  return r;
}
